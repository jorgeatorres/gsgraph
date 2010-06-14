/*
 * gs-graph.c
 * Copyright (C) 2007 Jorge Torres <jtorresh@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <math.h>

#include <gdk/gdkkeysyms.h>

#include "gs-debug.h"
#include "gs-graph.h"
#include "gs-misc.h"

#define GS_GRAPH_GET_PRIVATE(object) (G_TYPE_INSTANCE_GET_PRIVATE ((object), GS_TYPE_GRAPH, GsGraphPrivate))

G_DEFINE_TYPE (GsGraph, gs_graph, GTK_TYPE_DRAWING_AREA);

#define DEFAULT_GRID_COLOR "#dddddd"
#define DEFAULT_AXIS_COLOR "#494949"
#define DEFAULT_BACKGROUND_COLOR "#ffffff"

#define ZOOM_FACTOR 1.1

#define MAX_VIEW_WIDTH 100
#define MAX_VIEW_HEIGHT 100

#define EVENT_MASK (GDK_BUTTON1_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_SCROLL_MASK | GDK_KEY_PRESS_MASK)

enum {
	PLOT_ATTACHED,
	PLOT_DETACHED,
	PLOT_CHANGED,
	LAST_SIGNAL
};

static guint graph_signals[LAST_SIGNAL] = { 0 };

static double DASH_PATTERN[] = { 4.0, 0.0 };

struct _GsGraphPrivate
{
	GList *plots; /* GsPlot */

	GsGraphTransform transform;

	/* view */
	GsRectangle view;

	/* coordinates of the origin */
	GsPoint origin;

	/* Colors */
	GdkColor *grid_color;
	GdkColor *axes_color;
	GdkColor *background_color;

	/* Visualization options */
	gboolean show_legends;
	gboolean paint_background;

	/* Grid & axes */
	gboolean show_grid;
	gboolean dotted_grid;
	gdouble grid_dx; /* in real units */
	gdouble grid_dy;

	gboolean show_vertical_axis;
	gboolean show_horizontal_axis;
	gboolean show_ticks;

	gint tick_length;

	/* For drag & drop */
	GdkCursor *default_cursor;
	GdkCursor *drag_cursor;
	gdouble drag_start_x, drag_start_y;

	gint width;
	gint height;
};

static void
gs_graph_point_to_pixel (GsPoint *src, GsPoint *dest, gpointer data)
{
	GsGraph *graph = GS_GRAPH (data);

	dest->x = (src->x - graph->priv->view.x1) * (((gdouble) graph->priv->width) / (graph->priv->view.x2 - graph->priv->view.x1));
	dest->y = ((gdouble) graph->priv->height) - ((src->y - graph->priv->view.y1) * ((gdouble) graph->priv->height) / (graph->priv->view.y2 - graph->priv->view.y1));
}

static void
gs_graph_pixel_to_point (GsPoint *src, GsPoint *dest, gpointer data)
{
	GsGraph *graph = GS_GRAPH (data);
	GsPoint axis;

	gs_graph_point_to_pixel (&graph->priv->origin, &axis, graph);

	dest->x = (src->x - axis.x) * (graph->priv->view.x2 - graph->priv->view.x1) / ((gdouble) graph->priv->width);
	dest->y = ((gdouble) graph->priv->height - src->y - axis.y) * (graph->priv->view.y2 - graph->priv->view.y1) / ((gdouble) graph->priv->height);
}

static void
gs_graph_finalize (GObject *object)
{
	GsGraph *graph = GS_GRAPH (object);

	g_free (graph->priv->grid_color);
	g_free (graph->priv->axes_color);
	g_free (graph->priv->background_color);

	gdk_cursor_unref (graph->priv->default_cursor);
	gdk_cursor_unref (graph->priv->drag_cursor);

	G_OBJECT_CLASS (gs_graph_parent_class)->finalize (object);
}

static void
paint_grid_and_ticks (GsGraph *graph, cairo_t *cr)
{
	GsPoint origin;
	gs_point_to_pixel (&(graph->priv->transform), &(graph->priv->origin), &origin);

	gint k;
	gdouble pos;

	if (!graph->priv->show_grid && !graph->priv->show_ticks) {
		return;
	}

	cairo_set_line_width (cr, 0.5);

	// Horizontal grid lines/ticks
	gint start_y = (gint) (floor (graph->priv->view.y1 / graph->priv->grid_dy));
	gint end_y  = (gint) (floor (graph->priv->view.y2 / graph->priv->grid_dy));

	for (k = start_y; k <= end_y; k++) {
		gs_point_to_pixel_values (&(graph->priv->transform), 0, k * graph->priv->grid_dy, NULL, &pos);
		
		if (graph->priv->show_grid) {
			cairo_save (cr);

			if (graph->priv->dotted_grid) {
				cairo_set_dash (cr, DASH_PATTERN, 1, 0);
			}

			gdk_cairo_set_source_color (cr, graph->priv->grid_color);
			cairo_move_to (cr, 0, pos);
			cairo_line_to (cr, graph->priv->width, pos);
			cairo_stroke (cr);
			cairo_restore (cr);
		}

		if (graph->priv->show_vertical_axis && graph->priv->show_ticks) {
			gdk_cairo_set_source_color (cr, graph->priv->axes_color);
			cairo_move_to (cr, origin.x - graph->priv->tick_length, pos);
			cairo_line_to (cr, origin.x + graph->priv->tick_length, pos);
			cairo_stroke (cr);
		}
	}

	// Vertical grid lines/ticks
	gint start_x = (gint) (floor (graph->priv->view.x1 / graph->priv->grid_dx));
	gint end_x  = (gint) (floor (graph->priv->view.x2 / graph->priv->grid_dx));

	for (k = start_x; k <= end_x; k++) {
		gs_point_to_pixel_values (&(graph->priv->transform), k * graph->priv->grid_dx, 0, &pos, NULL);

		if (graph->priv->show_grid) {
			cairo_save (cr);

			if (graph->priv->dotted_grid) {
				cairo_set_dash (cr, DASH_PATTERN, 1, 0);
			}

			gdk_cairo_set_source_color (cr, graph->priv->grid_color);
			cairo_move_to (cr, pos, 0);
			cairo_line_to (cr, pos, graph->priv->height);
			cairo_stroke (cr);
			cairo_restore (cr);
		}

		if (graph->priv->show_horizontal_axis && graph->priv->show_ticks) {
			gdk_cairo_set_source_color (cr, graph->priv->axes_color);
			cairo_move_to (cr, pos, origin.y + graph->priv->tick_length);
			cairo_line_to (cr, pos, origin.y - graph->priv->tick_length);
			cairo_stroke (cr);
		}
	}
}

static void
paint_axes (GsGraph *graph, cairo_t *cr)
{
	GsPoint origin;
	gs_point_to_pixel (&(graph->priv->transform), &(graph->priv->origin), &origin);

	cairo_set_line_width (cr, 1.);
	gdk_cairo_set_source_color (cr, graph->priv->axes_color);

	if (graph->priv->show_horizontal_axis) {
		cairo_move_to (cr, 0, origin.y + 0.5);
		cairo_line_to (cr, graph->priv->width, origin.y + 0.5);
	}

	if (graph->priv->show_vertical_axis) {
		cairo_move_to (cr, origin.x + 0.5, 0);
		cairo_line_to (cr, origin.x + 0.5, graph->priv->height);
	}

	cairo_stroke (cr);
}

static void
paint_plots (GsGraph *graph, cairo_t *cr)
{
	GList *l = graph->priv->plots;

	while (l != NULL) {
		GsPlot *current_plot = (GsPlot *) l->data;

		cairo_save (cr);
		gs_plot_draw (current_plot, &(graph->priv->transform), &(graph->priv->view), cr);
		cairo_restore (cr);

		l = l->next;
	}

}

static void
gs_graph_plot_changed (GsPlot *plot, gpointer data)
{
	GsGraph *graph = GS_GRAPH (data);

	gtk_widget_queue_draw (GTK_WIDGET (graph));

	g_signal_emit_by_name (graph, "plot-changed", plot);
}

static gboolean
gs_graph_expose (GtkWidget *widget, GdkEventExpose *event)
{
	GsGraph *graph = GS_GRAPH (widget);
	cairo_t *cr;

	if (event->count > 0) {
		return FALSE;
	}

	gdk_window_clear (widget->window);
	cr = gdk_cairo_create (widget->window);

	/* Clip region
	cairo_rectangle (cr, 0, 0, graph->priv->width, graph->priv->height);
	cairo_clip (cr); */

	if (graph->priv->paint_background) {
		cairo_save (cr);
		gdk_cairo_set_source_color (cr, graph->priv->background_color);
		cairo_rectangle (cr, 0, 0, graph->priv->width, graph->priv->height);
		cairo_fill (cr);
		cairo_restore (cr);
	}

	cairo_save (cr);
	paint_axes (graph, cr);
	cairo_restore (cr);

	cairo_save (cr);
	paint_grid_and_ticks (graph, cr);
	cairo_restore (cr);

	cairo_save (cr);
	paint_plots (graph, cr);
	cairo_restore (cr);

	cairo_destroy (cr);

	return FALSE;
}

static gboolean
gs_graph_configure (GtkWidget *widget, GdkEventConfigure *event)
{
	GsGraph *graph = GS_GRAPH (widget);

	graph->priv->width = event->width;
	graph->priv->height = event->height;

	return FALSE;
}

#define KEY_MOVEMENT_STEP 0.5

static gboolean
gs_graph_key_press (GtkWidget *widget, GdkEventKey *event)
{
	GsGraph *graph = GS_GRAPH (widget);

	switch (event->keyval) {
		default:
			return FALSE;

		case GDK_Left:
		case GDK_KP_Left:
			graph->priv->view.x1 -= KEY_MOVEMENT_STEP;
			graph->priv->view.x2 -= KEY_MOVEMENT_STEP;
			break;
		case GDK_Right:
		case GDK_KP_Right:
			graph->priv->view.x1 += KEY_MOVEMENT_STEP;
			graph->priv->view.x2 += KEY_MOVEMENT_STEP;
			break;
		case GDK_Up:
		case GDK_KP_Up:
			graph->priv->view.y1 += KEY_MOVEMENT_STEP;
			graph->priv->view.y2 += KEY_MOVEMENT_STEP;
			break;
		case GDK_Down:
		case GDK_KP_Down:
			graph->priv->view.y1 -= KEY_MOVEMENT_STEP;
			graph->priv->view.y2 -= KEY_MOVEMENT_STEP;
			break;
	}

	gtk_widget_queue_draw (widget);

	return FALSE;
}

static gboolean
gs_graph_button_press (GtkWidget *widget, GdkEventButton *event)
{
	if (event->button != 1) {
		return FALSE;
	}
	
	GsGraph *graph = GS_GRAPH (widget);
		
	graph->priv->drag_start_x = event->x;
	graph->priv->drag_start_y = event->y;

	gdk_window_set_cursor (widget->window, graph->priv->drag_cursor);
	
	return FALSE;
}

static gboolean
gs_graph_button_release (GtkWidget *widget, GdkEventButton *event)
{
	if (event->button != 1) {
		return FALSE;
	}

	GsGraph *graph = GS_GRAPH (widget);
	gdk_window_set_cursor (widget->window, graph->priv->default_cursor);
	
	return FALSE;
}

static gboolean
gs_graph_motion_notify (GtkWidget *widget, GdkEventMotion *event)
{
	gdouble drag_start_x, drag_start_y;
	gdouble drag_end_x, drag_end_y;
	
	GsGraph *graph = GS_GRAPH (widget);

	gs_pixel_to_point_values (&(graph->priv->transform), graph->priv->drag_start_x, graph->priv->drag_start_y,
			       &drag_start_x, &drag_start_y);
	gs_pixel_to_point_values (&(graph->priv->transform), event->x, event->y, &drag_end_x, &drag_end_y);

	graph->priv->view.x1 = graph->priv->view.x1 - drag_end_x + drag_start_x;
	graph->priv->view.x2 = graph->priv->view.x2 - drag_end_x + drag_start_x;
	graph->priv->view.y1 = graph->priv->view.y1 - drag_end_y + drag_start_y;
	graph->priv->view.y2 = graph->priv->view.y2 - drag_end_y + drag_start_y;

	gtk_widget_queue_draw (widget);

	graph->priv->drag_start_x = event->x;
	graph->priv->drag_start_y = event->y;

	return TRUE;
}

static gboolean
gs_graph_scroll (GtkWidget *widget, GdkEventScroll *event)
{
	GsGraph *graph = GS_GRAPH (widget);

	gdouble view_width = graph->priv->view.x2 - graph->priv->view.x1;
	gdouble view_height = graph->priv->view.y2 - graph->priv->view.y1;
	gdouble view_center_x = graph->priv->view.x1 + view_width / 2.0;
	gdouble view_center_y = graph->priv->view.y1 + view_height / 2.0;

	if (event->direction == GDK_SCROLL_DOWN) {
		view_width *= ZOOM_FACTOR;
		view_height *= ZOOM_FACTOR;
	} else if (event->direction == GDK_SCROLL_UP) {
		view_width /= ZOOM_FACTOR;
		view_height /= ZOOM_FACTOR;
	}

	graph->priv->view.x1 = view_center_x - view_width / 2.0;
	graph->priv->view.x2 = view_center_x + view_width / 2.0;
	graph->priv->view.y1 = view_center_y - view_height / 2.0;
	graph->priv->view.y2 = view_center_y + view_height / 2.0;

	gtk_widget_queue_draw (widget);

	return FALSE;
}

static void
gs_graph_init (GsGraph *self)
{
	GTK_WIDGET_SET_FLAGS (GTK_WIDGET (self), GTK_CAN_FOCUS);
	gtk_widget_set_size_request (GTK_WIDGET (self), 500, 500);
	gtk_widget_add_events (GTK_WIDGET (self), EVENT_MASK);

	self->priv = GS_GRAPH_GET_PRIVATE (self);
	self->priv->transform.point_to_pixel = gs_graph_point_to_pixel;
	self->priv->transform.pixel_to_point = gs_graph_pixel_to_point;
	self->priv->transform.data = self;

	self->priv->origin.x = 0.0;
	self->priv->origin.y = 0.0;

	self->priv->view.x1 = -5.0;
	self->priv->view.x2 = 5.0;
	self->priv->view.y1 = -4.7;
	self->priv->view.y2 = 4.7;

	self->priv->plots = NULL;

	self->priv->show_legends = TRUE;
	self->priv->paint_background = TRUE;
	self->priv->show_grid = FALSE;
	self->priv->show_vertical_axis = TRUE;
	self->priv->show_horizontal_axis = TRUE;
	self->priv->show_ticks = TRUE;

	self->priv->grid_dx = 0.5;
	self->priv->grid_dy = 0.5;

	self->priv->tick_length = 4;

	self->priv->dotted_grid = FALSE;

	self->priv->grid_color = g_new0 (GdkColor, 1);
	self->priv->axes_color = g_new0 (GdkColor, 1);
	self->priv->background_color = g_new0 (GdkColor, 1);
	/* def colors */
	gdk_color_parse (DEFAULT_GRID_COLOR, self->priv->grid_color);
	gdk_color_parse (DEFAULT_AXIS_COLOR, self->priv->axes_color);
	gdk_color_parse (DEFAULT_BACKGROUND_COLOR, self->priv->background_color);

	self->priv->default_cursor = gdk_cursor_new (GDK_LEFT_PTR);
	self->priv->drag_cursor = gdk_cursor_new (GDK_FLEUR);
}

static void
gs_graph_class_init (GsGraphClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	object_class->finalize = gs_graph_finalize;

	widget_class->expose_event = gs_graph_expose;
	widget_class->configure_event = gs_graph_configure;
	widget_class->key_press_event = gs_graph_key_press;
	widget_class->button_press_event = gs_graph_button_press;
	widget_class->button_release_event = gs_graph_button_release;
	widget_class->motion_notify_event = gs_graph_motion_notify;
	widget_class->scroll_event = gs_graph_scroll;

	graph_signals[PLOT_ATTACHED] = g_signal_new ("plot-attached",
						     G_OBJECT_CLASS_TYPE (object_class),
						     G_SIGNAL_RUN_FIRST,
						     G_STRUCT_OFFSET (GsGraphClass, plot_attached),
						     NULL,
						     NULL,
						     g_cclosure_marshal_VOID__OBJECT,
						     G_TYPE_NONE,
						     1, GS_TYPE_PLOT);

	graph_signals[PLOT_ATTACHED] = g_signal_new ("plot-detached",
						     G_OBJECT_CLASS_TYPE (object_class),
						     G_SIGNAL_RUN_FIRST,
						     G_STRUCT_OFFSET (GsGraphClass, plot_detached),
						     NULL,
						     NULL,
						     g_cclosure_marshal_VOID__OBJECT,
						     G_TYPE_NONE,
						     1, GS_TYPE_PLOT);

	graph_signals[PLOT_ATTACHED] = g_signal_new ("plot-changed",
						     G_OBJECT_CLASS_TYPE (object_class),
						     G_SIGNAL_RUN_FIRST,
						     G_STRUCT_OFFSET (GsGraphClass, plot_changed),
						     NULL,
						     NULL,
						     g_cclosure_marshal_VOID__OBJECT,
						     G_TYPE_NONE,
						     1, GS_TYPE_PLOT);

	g_type_class_add_private (object_class, sizeof (GsGraphPrivate));
}

GtkWidget *
gs_graph_new (void)
{
	return GTK_WIDGET (g_object_new (GS_TYPE_GRAPH, NULL));
}

void
gs_graph_get_view (GsGraph *graph, GsRectangle *view)
{
	g_return_if_fail (GS_IS_GRAPH (graph));

	if (view != NULL) {
		view->x1 = graph->priv->view.x1;
		view->x2 = graph->priv->view.x2;
		view->y1 = graph->priv->view.y1;
		view->y2 = graph->priv->view.y2;
	}
}

void
gs_graph_set_view (GsGraph *graph, GsRectangle view)
{
	g_return_if_fail (GS_IS_GRAPH (graph));
	g_return_if_fail ((view.x1 != view.x2) && (view.y1 != view.y2));

	graph->priv->view.x1 = MIN (view.x1, view.x2);
	graph->priv->view.x2 = MAX (view.x1, view.x2);
	graph->priv->view.y1 = MIN (view.y1, view.y2);
	graph->priv->view.y2 = MAX (view.y1, view.y2);

	gtk_widget_queue_draw (GTK_WIDGET (graph));
}

void
gs_graph_set_paint_background (GsGraph *graph, gboolean paint_background)
{
	g_return_if_fail (GS_IS_GRAPH (graph));

	graph->priv->paint_background = paint_background;
	gtk_widget_queue_draw (GTK_WIDGET (graph));
}

void
gs_graph_set_background_color (GsGraph *graph, const GdkColor *color)
{
	g_return_if_fail (GS_IS_GRAPH (graph));
	g_return_if_fail (color != NULL);

	GdkColor *ncolor = gdk_color_copy (color);

	g_free (graph->priv->background_color);
	graph->priv->background_color = ncolor;

	if (graph->priv->paint_background) {
		gtk_widget_queue_draw (GTK_WIDGET (graph));
	}
}

void
gs_graph_set_show_grid (GsGraph *graph, gboolean show_grid)
{
	g_return_if_fail (GS_IS_GRAPH (graph));

	graph->priv->show_grid = show_grid;
	gtk_widget_queue_draw (GTK_WIDGET (graph));
}

void
gs_graph_set_grid_color (GsGraph *graph, const GdkColor *color)
{
	g_return_if_fail (GS_IS_GRAPH (graph));
	g_return_if_fail (color != NULL);

	GdkColor *ncolor = gdk_color_copy (color);

	g_free (graph->priv->grid_color);
	graph->priv->grid_color = ncolor;

	if (graph->priv->show_grid) {
		gtk_widget_queue_draw (GTK_WIDGET (graph));
	}
}

void
gs_graph_set_grid_dotted (GsGraph *graph, gboolean grid_dotted)
{
	g_return_if_fail (GS_IS_GRAPH (graph));

	graph->priv->dotted_grid = grid_dotted;

	if (graph->priv->show_grid) {
		gtk_widget_queue_draw (GTK_WIDGET (graph));
	}	
}

void
gs_graph_set_grid_horizontal_spacing (GsGraph *graph, gdouble grid_dx)
{
	g_return_if_fail (GS_IS_GRAPH (graph));

	graph->priv->grid_dx = grid_dx;

	if (graph->priv->show_grid) {
		gtk_widget_queue_draw (GTK_WIDGET (graph));
	}
}

void
gs_graph_set_grid_vertical_spacing (GsGraph *graph, gdouble grid_dy)
{
	g_return_if_fail (GS_IS_GRAPH (graph));

	graph->priv->grid_dy = grid_dy;

	if (graph->priv->show_grid) {
		gtk_widget_queue_draw (GTK_WIDGET (graph));
	}
}

void
gs_graph_set_show_vertical_axis (GsGraph *graph, gboolean show_vertical_axis)
{
	g_return_if_fail (GS_IS_GRAPH (graph));

	graph->priv->show_vertical_axis = show_vertical_axis;
	gtk_widget_queue_draw (GTK_WIDGET (graph));
}

void
gs_graph_set_show_horizontal_axis (GsGraph *graph, gboolean show_horizontal_axis)
{
	g_return_if_fail (GS_IS_GRAPH (graph));

	graph->priv->show_horizontal_axis = show_horizontal_axis;
	gtk_widget_queue_draw (GTK_WIDGET (graph));
}

void
gs_graph_set_axes_color (GsGraph *graph, const GdkColor *color)
{
	g_return_if_fail (GS_IS_GRAPH (graph));
	g_return_if_fail (color != NULL);

	GdkColor *ncolor = gdk_color_copy (color);

	g_free (graph->priv->axes_color);
	graph->priv->axes_color = ncolor;

	if (graph->priv->show_horizontal_axis || graph->priv->show_vertical_axis) {
		gtk_widget_queue_draw (GTK_WIDGET (graph));
	}
}

void
gs_graph_set_show_ticks (GsGraph *graph, gboolean show_ticks)
{
	g_return_if_fail (GS_IS_GRAPH (graph));

	graph->priv->show_ticks = show_ticks;
	gtk_widget_queue_draw (GTK_WIDGET (graph));
}

void
gs_graph_set_show_legends (GsGraph *graph, gboolean show_legends)
{
	g_return_if_fail (GS_IS_GRAPH (graph));

	graph->priv->show_legends = show_legends;
	gtk_widget_queue_draw (GTK_WIDGET (graph));
}

void
gs_graph_set_tick_length (GsGraph *graph, guint tick_length)
{
	g_return_if_fail (GS_IS_GRAPH (graph));

	graph->priv->tick_length = tick_length;

	if ((graph->priv->show_vertical_axis || graph->priv->show_horizontal_axis) && graph->priv->show_ticks) {
		gtk_widget_queue_draw (GTK_WIDGET (graph));
	}
}

/*
void
gs_graph_remove_plots (GsGraph *graph)
{
	g_return_if_fail (GS_IS_GRAPH (graph));

	if (graph->priv->plots != NULL) {
		g_list_foreach (graph->priv->plots, (GFunc) gs_plot_free, NULL);
		g_list_free (graph->priv->plots);
		graph->priv->plots = NULL;

		gtk_widget_queue_draw (GTK_WIDGET (graph));
	}
}*/

void
gs_graph_attach_plot (GsGraph *graph, GsPlot *plot)
{
	g_return_if_fail (GS_IS_GRAPH (graph));
	g_return_if_fail (GS_IS_PLOT (plot));

	graph->priv->plots = g_list_prepend (graph->priv->plots, plot);

	g_signal_connect_after (G_OBJECT (plot),
				"plot-changed",
				G_CALLBACK (gs_graph_plot_changed),
				graph);

	gtk_widget_queue_draw (GTK_WIDGET (graph));

	g_signal_emit_by_name (graph, "plot-attached", plot);
}
