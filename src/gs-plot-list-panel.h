/*
 * gs-function-plot-dialog.h
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

#ifndef __GS_PLOT_LIST_PANEL_H__
#define __GS_PLOT_LIST_PANEL_H__

#include <glib-object.h>
#include <gtk/gtk.h>
#include "gs-window.h"

G_BEGIN_DECLS

#define	GS_TYPE_PLOT_LIST_PANEL			(gs_plot_list_panel_get_type())
#define	GS_PLOT_LIST_PANEL(obj)			(G_TYPE_CHECK_INSTANCE_CAST((obj), GS_TYPE_PLOT_LIST_PANEL, GsPlotListPanel))
#define GS_PLOT_LIST_PANEL_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), GS_TYPE_PLOT_LIST_PANEL, GsPlotListPanelClass))
#define GS_IS_PLOT_LIST_PANEL(obj)		(G_TYPE_CHECK_INSTANCE_TYPE((obj), GS_TYPE_PLOT_LIST_PANEL))
#define GS_IS_PLOT_LIST_PANEL_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), GS_TYPE_PLOT_LIST_PANEL))
#define GS_PLOT_LIST_PANEL_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), GS_TYPE_PLOT_LIST_PANEL, GsPlotListPanelClass))

typedef struct _GsPlotListPanel GsPlotListPanel;
typedef struct _GsPlotListPanelPrivate GsPlotListPanelPrivate;
typedef struct _GsPlotListPanelClass GsPlotListPanelClass;

struct _GsPlotListPanel
{
	GtkVBox parent;

	GsPlotListPanelPrivate *priv;
};

struct _GsPlotListPanelClass
{
	GtkVBoxClass parent_class;
};

GType		gs_plot_list_panel_get_type (void) G_GNUC_CONST;

GtkWidget	*gs_plot_list_panel_new (GsWindow *window);

G_END_DECLS

#endif /* __GS_PLOT_LIST_PANEL_H__ */

