/*
 * gs-window.h
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

#ifndef __GS_WINDOW_H__
#define __GS_WINDOW_H__

#include <glib-object.h>
#include <gtk/gtk.h>

#include "gs-graph.h"

G_BEGIN_DECLS

#define	GS_TYPE_WINDOW			(gs_window_get_type())
#define	GS_WINDOW(obj)			(G_TYPE_CHECK_INSTANCE_CAST((obj), GS_TYPE_WINDOW, GsWindow))
#define GS_WINDOW_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), GS_TYPE_WINDOW, GsWindowClass))
#define GS_IS_WINDOW(obj)		(G_TYPE_CHECK_INSTANCE_TYPE((obj), GS_TYPE_WINDOW))
#define GS_IS_WINDOW_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), GS_TYPE_WINDOW))
#define GS_WINDOW_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), GS_TYPE_WINDOW, GsWindowClass))

typedef struct _GsWindow GsWindow;
typedef struct _GsWindowPrivate GsWindowPrivate;
typedef struct _GsWindowClass GsWindowClass;

struct _GsWindow
{
	GtkWindow parent;

	GsWindowPrivate *priv;
};

struct _GsWindowClass
{
	GtkWindowClass parent_class;
};

GType		gs_window_get_type (void) G_GNUC_CONST;

GtkWidget	*gs_window_new (void);

GsGraph		*gs_window_get_graph (GsWindow *window);

G_END_DECLS

#endif /* __GS_WINDOW_H__ */
