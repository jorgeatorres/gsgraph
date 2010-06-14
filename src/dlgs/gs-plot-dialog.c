/*
 * gs-function-plot-dialog.c
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

#include "gs-plot-dialog.h"

#define GS_PLOT_DIALOG_GET_PRIVATE(object) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((object), GS_TYPE_PLOT_DIALOG, GsPlotDialogPrivate))

G_DEFINE_TYPE (GsPlotDialog, gs_plot_dialog, GTK_TYPE_DIALOG);

enum {
	PROP_0,
	PROP_

struct _GsPlotDialogPrivate {
	GtkWidget *equation_entry;
};

static void equation_entry_changed (GtkEntry *entry, GsPlotDialog *dialog)
{
	gchar *equation = g_strchug(g_strdup (gtk_entry_get_text (entry)));

	g_return_if_fail (equation != NULL);

	gtk_dialog_set_response_sensitive (GTK_DIALOG (dialog), GTK_RESPONSE_OK, (*equation != '\0'));

	g_free (equation);
}

static void
gs_plot_dialog_init (GsPlotDialog *dlg)
{
	dlg->priv = GS_PLOT_DIALOG_GET_PRIVATE (dlg);

	gtk_window_set_resizable (GTK_WINDOW (dlg), FALSE);
	gtk_window_set_destroy_with_parent (GTK_WINDOW (dlg), TRUE);
	gtk_dialog_add_buttons (GTK_DIALOG (dlg), GTK_STOCK_OK, GTK_RESPONSE_OK,
						   GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
	gtk_dialog_set_response_sensitive (GTK_DIALOG (dlg), GTK_RESPONSE_OK, FALSE);
	gtk_dialog_set_default_response (GTK_DIALOG (dlg), GTK_RESPONSE_OK);

	dlg->priv->equation_entry = gtk_entry_new ();
	gtk_entry_set_activates_default (GTK_ENTRY (dlg->priv->equation_entry), TRUE);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dlg)->vbox), dlg->priv->equation_entry);
	gtk_widget_show (dlg->priv->equation_entry);

	g_signal_connect (dlg->priv->equation_entry, "changed",
			  G_CALLBACK (equation_entry_changed),
			  dlg);
}

static void
gs_plot_dialog_class_init (GsPlotDialogClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (object_class, sizeof (GsPlotDialogPrivate));
}

GtkWidget *
gs_plot_dialog_new (GtkWindow *parent, GsPlotDialogAction action)
{
	GsPlotDialog *dlg;

	dlg = g_object_new (GS_TYPE_PLOT_DIALOG, NULL);
	
	if (parent != NULL) {
		gtk_window_set_transient_for (GTK_WINDOW (dlg), parent);
	}

	gs_plot_dialog_set_action (dlg, action);

	return GTK_WIDGET (dlg);
}

/*
gchar *
gs_plot_dialog_get_equation (GsPlotDialog *dialog)
{
	g_return_val_if_fail (GS_IS_PLOT_DIALOG (dialog), NULL);

	return g_strstrip (g_strdup (gtk_entry_get_text (GTK_ENTRY (dialog->priv->equation_entry))));
}

// TODO - Check if this updates the sensibility of GTK_RESPONSE_OK or we have to do it manually 
void
gs_plot_dialog_set_equation (GsPlotDialog *dialog, const gchar *equation)
{
	g_return_if_fail (GS_IS_PLOT_DIALOG (dialog));

	if (equation == NULL) {
		gtk_editable_delete_text (GTK_EDITABLE (dialog->priv->equation_entry), 0, -1);
	} else {
		gtk_entry_set_text (GTK_ENTRY (dialog->priv->equation_entry), equation);
	}
}*/
