/*
 * gs-plot.c
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

#include "gs-plot.h"
#include "gs-plot-private.h"

#define GS_PLOT_GET_PRIVATE(object) (G_TYPE_INSTANCE_GET_PRIVATE ((object), GS_TYPE_PLOT, GsPlotPrivate))

G_DEFINE_ABSTRACT_TYPE (GsPlot, gs_plot, G_TYPE_OBJECT);

enum {
	PLOT_CHANGED,
	LAST_SIGNAL
};

static guint plot_signals[LAST_SIGNAL] = { 0 };
static guint unnamed_plot_n = 1;

static const gchar *DEFAULT_COLORS[] = { "#000", "blue", "red", "green", "violet" };

static void
gs_plot_finalize (GObject *object)
{
	GsPlot *plot = GS_PLOT (object);

	g_free (plot->priv->name);

	G_OBJECT_CLASS (gs_plot_parent_class)->finalize (object);
}

static void
gs_plot_init (GsPlot *self)
{
	self->priv = GS_PLOT_GET_PRIVATE (self);
	self->priv->name = g_strdup_printf ("Plot %d", ++unnamed_plot_n);
	
	gdk_color_parse (DEFAULT_COLORS[(unnamed_plot_n % 5)], &(self->priv->color));
}

static void
gs_plot_class_init (GsPlotClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = gs_plot_finalize;

	/* Signals */
	plot_signals[PLOT_CHANGED] = g_signal_new ("plot-changed",
						   G_OBJECT_CLASS_TYPE (object_class),
						   G_SIGNAL_RUN_LAST,
						   G_STRUCT_OFFSET (GsPlotClass, plot_changed),
						   NULL,
						   NULL,
						   g_cclosure_marshal_VOID__VOID,
						   G_TYPE_NONE,
						   0);

	g_type_class_add_private (object_class, sizeof (GsPlotPrivate));

}

const gchar *
gs_plot_get_name (GsPlot *plot)
{
	return (const gchar *) plot->priv->name;
}

void
gs_plot_set_name (GsPlot *plot, const gchar *name)
{
	g_return_if_fail (name != NULL);

	g_free (plot->priv->name);
	plot->priv->name = g_strdup (name);
}

void
gs_plot_set_color (GsPlot *plot, const GdkColor *color)
{
	g_return_if_fail (GS_IS_PLOT (plot));
	g_return_if_fail (color != NULL);

	plot->priv->color = *color;
	
	g_signal_emit_by_name (G_OBJECT (plot), "plot-changed");
}

GdkColor *
gs_plot_get_color (GsPlot *plot)
{
	g_return_if_fail (GS_IS_PLOT (plot));
	
	return gdk_color_copy (&(plot->priv->color));
}

void
gs_plot_draw (GsPlot *plot, GsGraphTransform *t, GsRectangle *v, cairo_t *cr)
{
	g_return_if_fail (GS_IS_PLOT (plot));

	GS_PLOT_GET_CLASS (plot)->draw_plot (plot, t, v, cr);
}
