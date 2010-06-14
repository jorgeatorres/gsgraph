/*
 * gs-plot.h
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

#ifndef __GS_FUNCTION_PLOT_H__
#define __GS_FUNCTION_PLOT_H__

#include <glib.h>
#include <glib-object.h>

#include "gs-plot.h"

G_BEGIN_DECLS

#define	GS_TYPE_FUNCTION_PLOT			(gs_function_plot_get_type())
#define	GS_FUNCTION_PLOT(obj)			(G_TYPE_CHECK_INSTANCE_CAST((obj), GS_TYPE_FUNCTION_PLOT, GsFunctionPlot))
#define GS_FUNCTION_PLOT_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), GS_TYPE_FUNCTION_PLOT, GsFunctionPlotClass))
#define GS_IS_FUNCTION_PLOT(obj)		(G_TYPE_CHECK_INSTANCE_TYPE((obj), GS_TYPE_FUNCTION_PLOT))
#define GS_IS_FUNCTION_PLOT_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), GS_TYPE_FUNCTION_PLOT))
#define GS_FUNCTION_PLOT_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS((obj), GS_TYPE_FUNCTION_PLOT, GsFunctionPlotClass))

typedef struct _GsFunctionPlot GsFunctionPlot;
typedef struct _GsFunctionPlotPrivate GsFunctionPlotPrivate;
typedef struct _GsFunctionPlotClass GsFunctionPlotClass;

struct _GsFunctionPlot
{
	GsPlot parent;

	GsFunctionPlotPrivate *priv;
};

struct _GsFunctionPlotClass
{
	GsPlotClass parent_class;
};

GType		gs_function_plot_get_type (void) G_GNUC_CONST;

GsPlot		*gs_function_plot_new (void);
GsPlot		*gs_function_plot_new_with_formula (const gchar *formula);

void	 	gs_function_plot_set_formula (GsFunctionPlot *plot, const gchar *formula);
gchar		*gs_function_plot_get_formula (GsFunctionPlot *plot);

G_END_DECLS

#endif /* __GS_FUNCTION_PLOT_H__ */
