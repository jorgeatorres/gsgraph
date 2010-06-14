/*
 * gs-dataset-plot.h
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

#ifndef __GS_DATASET_PLOT_H__
#define __GS_DATASET_PLOT_H__

#include <glib.h>
#include <glib-object.h>

#include "gs-plot.h"

G_BEGIN_DECLS

#define	GS_TYPE_DATASET_PLOT			(gs_dataset_plot_get_type())
#define	GS_DATASET_PLOT(obj)			(G_TYPE_CHECK_INSTANCE_CAST((obj), GS_TYPE_DATASET_PLOT, GsDatasetPlot))
#define GS_DATASET_PLOT_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), GS_TYPE_DATASET_PLOT, GsDatasetPlotClass))
#define GS_IS_DATASET_PLOT(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), GS_TYPE_DATASET_PLOT))
#define GS_IS_DATASET_PLOT_CLASS(klass)		(G_TYPE_CHECK_CLASS_TYPE((klass), GS_TYPE_DATASET_PLOT))
#define GS_DATASET_PLOT_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS((obj), GS_TYPE_DATASET_PLOT, GsDatasetPlotClass))

typedef struct _GsDatasetPlot GsDatasetPlot;
typedef struct _GsDatasetPlotPrivate GsDatasetPlotPrivate;
typedef struct _GsDatasetPlotClass GsDatasetPlotClass;

struct _GsDatasetPlot
{
	GsPlot parent;

	GsDatasetPlotPrivate *priv;
};

struct _GsDatasetPlotClass
{
	GsPlotClass parent_class;
};

GType		gs_dataset_plot_get_type (void) G_GNUC_CONST;

GsPlot		*gs_dataset_plot_new (void);
GsPlot		*gs_dataset_plot_new_with_data (gdouble x[], gdouble y[], guint n);

void	 	gs_dataset_plot_set_data (GsDatasetPlot *plot, gdouble x[], gdouble y[], guint n);

// TODO - Support for 'exotic' point types: diamonds, etc.
// TODO - Support for joining the points with lines. Check out other dataset-plotting tools
// TODO - Support for marking the trail leading to the point, something like the following:
// -------(point)
//           |
//           |
//           |

G_END_DECLS

#endif /* __GS_DATASET_PLOT_H__ */
