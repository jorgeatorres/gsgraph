/*
 * gs-graph.h
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

#ifndef __GS_GRAPH_H__
#define __GS_GRAPH_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>

#include "gs-plot.h"

G_BEGIN_DECLS

#define GS_TYPE_GRAPH			(gs_graph_get_type())
#define GS_GRAPH(obj)			(G_TYPE_CHECK_INSTANCE_CAST((obj), GS_TYPE_GRAPH, GsGraph))
#define GS_GRAPH_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), GS_TYPE_GRAPH, GsGraphClass))
#define GS_IS_GRAPH(obj)		(G_TYPE_CHECK_INSTANCE_TYPE((obj), GS_TYPE_GRAPH))
#define GS_IS_GRAPH_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), GS_TYPE_GRAPH))
#define GS_GRAPH_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS((obj), GS_TYPE_GRAPH, GsGraphClass))

typedef struct _GsGraph GsGraph;
typedef struct _GsGraphClass GsGraphClass;
typedef struct _GsGraphPrivate GsGraphPrivate;

struct _GsGraph
{
	GtkDrawingArea parent;

	/* <private> */
	GsGraphPrivate *priv;
};

struct _GsGraphClass
{
	GtkDrawingAreaClass parent_class;

	void (*plot_attached) (GsGraph *graph, GsPlot *plot);
	void (*plot_detached) (GsGraph *graph, GsPlot *plot);
	void (*plot_changed) (GsGraph *graph, GsPlot *plot);
};

GType		gs_graph_get_type (void) G_GNUC_CONST;

GtkWidget	*gs_graph_new (void);

void		gs_graph_get_view (GsGraph *graph, GsRectangle *view);
void		gs_graph_set_view (GsGraph *graph, GsRectangle view);

void		gs_graph_set_paint_background (GsGraph *graph, gboolean	paint_background);
void		gs_graph_set_background_color (GsGraph *graph, const GdkColor *color);

void		gs_graph_set_show_grid (GsGraph *graph, gboolean show_grid);
void		gs_graph_set_grid_color (GsGraph *graph, const GdkColor *color);
void		gs_graph_set_grid_dotted (GsGraph *graph, gboolean grid_dotted);

void		gs_graph_set_grid_horizontal_spacing (GsGraph *graph, gdouble grid_dx);
void		gs_graph_set_grid_vertical_spacing (GsGraph *graph, gdouble grid_dy);

void		gs_graph_set_vertical_axis (GsGraph *graph, gboolean show_vertical_axis);
void		gs_graph_set_show_horizontal_axis (GsGraph *graph, gboolean show_horizontal_axis);
void		gs_graph_set_axes_color (GsGraph *graph, const GdkColor *color);

void		gs_graph_set_show_ticks (GsGraph *graph, gboolean show_ticks);
void		gs_graph_set_show_legends (GsGraph *graph, gboolean show_legends);

void		gs_graph_set_tick_length (GsGraph *graph, guint tick_length);

void		gs_graph_attach_plot (GsGraph *graph, GsPlot *plot);


G_END_DECLS

#endif /* __GS_GRAPH_H__ */
