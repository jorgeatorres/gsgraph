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

#include "gs-debug.h"
#include "gs-plot-list-panel.h"
#include "dlgs/gs-plot-dialog.h"
#include "gs-function-plot.h"

#define GS_PLOT_LIST_PANEL_GET_PRIVATE(object) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((object), GS_TYPE_PLOT_LIST_PANEL, GsPlotListPanelPrivate))

G_DEFINE_TYPE (GsPlotListPanel, gs_plot_list_panel, GTK_TYPE_VBOX);

enum {
	PROP_0,
	PROP_WINDOW,
};

struct _GsPlotListPanelPrivate {
	GsWindow *window;

	GtkWidget *treeview;
	GtkTreeModel *treemodel;
};

static void
gs_plot_list_panel_finalize (GObject *object)
{
	GsPlotListPanel *panel = GS_PLOT_LIST_PANEL (object);

	/* TODO - disconnect stuff */
}

static void
new_plot_button_clicked (GtkButton *button, GsPlotListPanel *panel)
{
	GtkWidget *dlg = gs_plot_dialog_new (GTK_WINDOW (panel->priv->window), GS_PLOT_DIALOG_ACTION_CREATE);

	gint res = gtk_dialog_run (GTK_DIALOG (dlg));

	switch (res) {
		default:
			break;
	}

	gtk_widget_destroy (dlg);
}

static void
plot_attached (GsGraph *graph, GsPlot *plot, GsPlotListPanel *panel)
{
	GtkTreeIter iter;
	gchar *name;

	LOG();

	gtk_list_store_append (GTK_LIST_STORE (panel->priv->treemodel), &iter);

	/* TODO - Hacer mas generico esto: si una funcion no tiene nombre, su nombre es la ecuacion que la define
	 * ... asi se querria en casi todos los casos */
	if (GS_IS_FUNCTION_PLOT (plot)) {
		name = gs_function_plot_get_formula (GS_FUNCTION_PLOT (plot));
	} else {
		name = g_strdup (gs_plot_get_name (plot));
	}

	gtk_list_store_set (GTK_LIST_STORE (panel->priv->treemodel), &iter,
			    0, NULL,
			    1, name,
			    -1);

	g_free (name);
}

static void
set_window (GsPlotListPanel *panel, GsWindow *window)
{
	g_return_if_fail (panel->priv->window == NULL);
	g_return_if_fail (GS_IS_WINDOW (window));

	panel->priv->window = window;

	g_signal_connect (gs_window_get_graph (panel->priv->window), "plot-attached",
			  G_CALLBACK (plot_attached), panel);
}

static void
gs_plot_list_panel_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	GsPlotListPanel *panel = GS_PLOT_LIST_PANEL (object);

	switch (prop_id) {
		case PROP_WINDOW:
			g_value_set_object (value, panel->priv->window);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gs_plot_list_panel_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	GsPlotListPanel *panel = GS_PLOT_LIST_PANEL (object);

	switch (prop_id) {
		case PROP_WINDOW:
			set_window (panel, g_value_get_object (value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gs_plot_list_panel_init (GsPlotListPanel *panel)
{
	GtkWidget *frame, *but;
	GtkWidget *hbox;

	panel->priv = GS_PLOT_LIST_PANEL_GET_PRIVATE (panel);
	panel->priv->treemodel = GTK_TREE_MODEL (gtk_list_store_new (2, GDK_TYPE_PIXBUF, G_TYPE_STRING));
	
	panel->priv->treeview = gtk_tree_view_new_with_model (panel->priv->treemodel);
	g_object_unref (panel->priv->treemodel);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (panel->priv->treeview), FALSE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (panel->priv->treeview),
				     gtk_tree_view_column_new_with_attributes ("Plot Color",
					     				       gtk_cell_renderer_pixbuf_new (),
									       NULL));
	gtk_tree_view_append_column (GTK_TREE_VIEW (panel->priv->treeview),
				     gtk_tree_view_column_new_with_attributes ("Plot Name",
					     				       gtk_cell_renderer_text_new (),
									       "text", 1,
									       NULL));

	frame = gtk_frame_new ("Current Plots");
	gtk_container_add (GTK_CONTAINER (frame), panel->priv->treeview);
	gtk_widget_show (panel->priv->treeview);

	gtk_box_pack_start (GTK_BOX (panel), frame, TRUE, TRUE, 5);
	gtk_widget_show (frame);

	hbox = gtk_hbox_new (TRUE, 2);
	gtk_box_pack_start (GTK_BOX (panel), hbox, FALSE, FALSE, 0);
	gtk_widget_show (hbox);

	but = gtk_button_new_with_label ("New Plot...");
	gtk_button_set_image (GTK_BUTTON (but), gtk_image_new_from_stock (GTK_STOCK_ADD,
									  GTK_ICON_SIZE_BUTTON));
//	gtk_button_set_label (GTK_BUTTON (but), "New Plot...");
	g_signal_connect (but, "clicked", G_CALLBACK (new_plot_button_clicked), panel);
	gtk_box_pack_start (GTK_BOX (hbox), but, TRUE, TRUE, 0);
	gtk_widget_show (but);

	gtk_widget_set_size_request (GTK_WIDGET (panel), 100, -1);
}

static void
gs_plot_list_panel_class_init (GsPlotListPanelClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	object_class->finalize = gs_plot_list_panel_finalize;
	object_class->get_property = gs_plot_list_panel_get_property;
	object_class->set_property = gs_plot_list_panel_set_property;

	g_object_class_install_property (object_class,
				         PROP_WINDOW,
					 g_param_spec_object ("window",
						 	      "Window",
							      "The GsWindow object this panel is associated with",
							      GS_TYPE_WINDOW,
							      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));


	g_type_class_add_private (object_class, sizeof (GsPlotListPanelPrivate));
}

GtkWidget *
gs_plot_list_panel_new (GsWindow *window)
{
	GsPlotListPanel *panel;

	panel = g_object_new (GS_TYPE_PLOT_LIST_PANEL, "window", window, NULL);

	return GTK_WIDGET (panel);
}
