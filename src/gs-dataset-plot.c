/*
 * gs-dataset-plot.c
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

#include <matheval.h>
#include <math.h>

#include "gs-debug.h"
#include "gs-plot-private.h"
#include "gs-dataset-plot.h"
/*#include "gs-misc.h"*/

struct _GsDatasetPlotPrivate
{
	GSList *dataset;
};

G_DEFINE_TYPE (GsDatasetPlot, gs_dataset_plot, GS_TYPE_PLOT);

// TODO - This should be defined in terms of how 'far' are we from the point and not hardcoded
#define POINT_RADIUS 4

static void
gs_dataset_plot_draw (GsPlot *plot, GsGraphTransform *t, GsRectangle *v, cairo_t *cr)
{
	GSList *l = GS_DATASET_PLOT(plot)->priv->dataset;
	gdouble x, y;

	cairo_save (cr);
	gdk_cairo_set_source_color (cr, &(plot->priv->color));

	while (l != NULL) {
		GsPoint *p = (GsPoint *) l->data;
		
		gs_point_to_pixel_values (t, p->x, p->y, &x, &y);

		cairo_arc (cr, x - POINT_RADIUS / 2, y - POINT_RADIUS / 2, POINT_RADIUS, 0, M_PI * 2);
		cairo_fill (cr);
		
		l = l->next;
	}

	cairo_restore (cr);
}

static void
gs_dataset_plot_finalize (GObject *object)
{
	GsDatasetPlot *plot = GS_DATASET_PLOT (object);

	if (plot->priv->dataset != NULL) {
		// free points
	}

	G_OBJECT_CLASS (gs_dataset_plot_parent_class)->finalize (object);
}

static void
gs_dataset_plot_init (GsDatasetPlot *self)
{
	self->priv = g_new0 (GsDatasetPlotPrivate, 1);
	self->priv->dataset = NULL;
}

static void
gs_dataset_plot_class_init (GsDatasetPlotClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GsPlotClass *plot_class = GS_PLOT_CLASS (klass);

	object_class->finalize = gs_dataset_plot_finalize;
	plot_class->draw_plot = gs_dataset_plot_draw;
}

GsPlot *
gs_dataset_plot_new (void)
{
	GsPlot *ret = GS_PLOT (g_object_new (GS_TYPE_DATASET_PLOT,
					     NULL));

	return ret;
}

GsPlot *
gs_dataset_plot_new_with_data (gdouble x[], gdouble y[], guint n)
{
	GsPlot *ret = gs_dataset_plot_new ();
	gs_dataset_plot_set_data (GS_DATASET_PLOT (ret), x, y, n);

	return ret;
}

void
gs_dataset_plot_set_data (GsDatasetPlot *plot, gdouble x[], gdouble y[], guint n)
{
	g_return_if_fail (GS_IS_DATASET_PLOT (plot));
	g_return_if_fail (x != NULL && y != NULL);

	gint i;

	for (i = 0; i < n; i++) {
		GsPoint *p = g_new0 (GsPoint, 1);
		p->x = x[i];
		p->y = y[i];

		plot->priv->dataset = g_slist_prepend (plot->priv->dataset, p);
	}
}
