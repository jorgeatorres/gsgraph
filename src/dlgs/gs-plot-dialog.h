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

#ifndef __GS_PLOT_DIALOG_H__
#define __GS_PLOT_DIALOG_H__

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define	GS_TYPE_PLOT_DIALOG		(gs_plot_dialog_get_type())
#define	GS_PLOT_DIALOG(obj)		(G_TYPE_CHECK_INSTANCE_CAST((obj), GS_TYPE_PLOT_DIALOG, GsPlotDialog))
#define GS_PLOT_DIALOG_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass), GS_TYPE_PLOT_DIALOG, GsPlotDialogClass))
#define GS_IS_PLOT_DIALOG(obj)		(G_TYPE_CHECK_INSTANCE_TYPE((obj), GS_TYPE_PLOT_DIALOG))
#define GS_IS_PLOT_DIALOG_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), GS_TYPE_PLOT_DIALOG))
#define GS_PLOT_DIALOG_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), GS_TYPE_PLOT_DIALOG, GsPlotDialogClass))

typedef struct _GsPlotDialog GsPlotDialog;
typedef struct _GsPlotDialogPrivate GsPlotDialogPrivate;
typedef struct _GsPlotDialogClass GsPlotDialogClass;

typedef enum {
	GS_PLOT_DIALOG_ACTION_CREATE,
	GS_PLOT_DIALOG_ACTION_EDIT
} GsPlotDialogAction;

struct _GsPlotDialog
{
	GtkDialog parent;

	GsPlotDialogPrivate *priv;
};

struct _GsPlotDialogClass
{
	GtkDialogClass parent_class;
};

GType		gs_plot_dialog_get_type (void) G_GNUC_CONST;

GtkWidget	*gs_plot_dialog_new (GtkWindow *parent, GsPlotDialogAction action);

void			gs_plot_dialog_set_action (GsPlotDialog *dialog, GsPlotDialogAction action);
GsPlotDialogAction	gs_plot_dialog_get_action (GsPlotDialog *dialog);

/*gchar		*gs_plot_dialog_get_equation (GsPlotDialog *dialog);
void		gs_plot_dialog_set_equation (GsPlotDialog *dialog, const gchar *equation);*/

G_END_DECLS

#endif /* __GS_PLOT_DIALOG_H__ */

