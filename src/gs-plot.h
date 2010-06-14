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

#ifndef __GS_PLOT_H__
#define __GS_PLOT_H__

#include <glib.h>
#include <glib-object.h>
#include <cairo/cairo.h>
#include <gdk/gdk.h>

#include "gs-misc.h"

G_BEGIN_DECLS

#define	GS_TYPE_PLOT		(gs_plot_get_type())
#define	GS_PLOT(obj)		(G_TYPE_CHECK_INSTANCE_CAST((obj), GS_TYPE_PLOT, GsPlot))
#define GS_PLOT_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass), GS_TYPE_PLOT, GsPlotClass))
#define GS_IS_PLOT(obj)		(G_TYPE_CHECK_INSTANCE_TYPE((obj), GS_TYPE_PLOT))
#define GS_IS_PLOT_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), GS_TYPE_PLOT))
#define GS_PLOT_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), GS_TYPE_PLOT, GsPlotClass))

typedef struct _GsPlot GsPlot;
typedef struct _GsPlotPrivate GsPlotPrivate;
typedef struct _GsPlotClass GsPlotClass;

struct _GsPlot
{
	GObject parent;

	GsPlotPrivate *priv;
};

struct _GsPlotClass
{
	GObjectClass parent_class;

	/* <protected> */
	void (*draw_plot) (GsPlot *plot, GsGraphTransform *t, GsRectangle *r, cairo_t *cr);
	void (*plot_changed) (GsPlot *plot);
};

GType		gs_plot_get_type (void) G_GNUC_CONST;

const gchar	*gs_plot_get_name (GsPlot *plot);
void		gs_plot_set_name (GsPlot *plot, const gchar *name);

void		gs_plot_set_color (GsPlot *plot, const GdkColor *color);
GdkColor	*gs_plot_get_color (GsPlot *plot);

/* maybe <protected> ? */
void		gs_plot_draw (GsPlot *plot, GsGraphTransform *t, GsRectangle *r, cairo_t *cr);

G_END_DECLS

#endif /* __GS_PLOT_H__ */
