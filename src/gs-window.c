/*
 * gs-window.c
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

#include "gs-window.h"
#include "gs-window-private.h"
#include "gs-action-callbacks.h"
#include "gs-plot-list-panel.h"

#define GS_WINDOW_GET_PRIVATE(object) (G_TYPE_INSTANCE_GET_PRIVATE ((object), GS_TYPE_WINDOW, GsWindowPrivate))

G_DEFINE_TYPE (GsWindow, gs_window, GTK_TYPE_WINDOW);

static void create_menubar (GsWindow *window, GtkWidget *box);

static const GtkActionEntry action_entries[] = {
	{ "GsGraph", NULL, "_GsGraph" },
	{ "View", NULL, "_View" },
	{ "Help", NULL, "_Help" },

	{ "GsGraph_Quit", GTK_STOCK_QUIT, "_Quit", NULL,
	  "Quit GsGraph", gtk_main_quit },

	{ "Help_About", GTK_STOCK_ABOUT, "_About", NULL,
	  "About this application", G_CALLBACK (_gs_cb_help_about) },
};

static const GtkToggleActionEntry toggle_action_entries[] = {
	{ "View_ShowGrid", NULL, "Show _Grid", NULL, "...",
	  G_CALLBACK (_gs_cb_view_show_grid), FALSE },

	{ "View_ShowPlotsPanel", NULL, "Show _Plots Panel", NULL, "...",
	  G_CALLBACK (_gs_cb_view_show_plots_panel), TRUE },
};

static void
gs_window_finalize (GObject *object)
{
/*	GsWindow *window = GS_WINDOW (object);

	g_free (window->priv);*/

	G_OBJECT_CLASS (gs_window_parent_class)->finalize (object);
}

#define UI_DEFINITION "<ui> \
  <menubar name=\"MainMenu\"> \
    <menu action=\"GsGraph\">  \
      <menuitem action=\"GsGraph_Quit\"/> \
    </menu> \
    <menu action=\"View\"> \
      <menuitem action=\"View_ShowPlotsPanel\"/> \
      <separator/> \
      <menuitem action=\"View_ShowGrid\"/> \
    </menu> \
    <menu action=\"Help\"> \
      <separator/> \
      <menuitem action=\"Help_About\"/> \
    </menu> \
  </menubar> \
</ui>\0"

static void
gs_window_build_ui (GsWindow *window)
{
	GtkWidget *menubar;

	g_return_if_fail (GS_IS_WINDOW (window));

	window->priv->box = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window), window->priv->box);
	gtk_widget_show (window->priv->box);

	window->priv->ui_manager = gtk_ui_manager_new ();

	window->priv->ui_actions = gtk_action_group_new ("MenuActions");
	gtk_action_group_add_actions (window->priv->ui_actions,
				      action_entries,
				      G_N_ELEMENTS (action_entries),
				      window);
	gtk_action_group_add_toggle_actions (window->priv->ui_actions,
					     toggle_action_entries,
					     G_N_ELEMENTS (toggle_action_entries),
					     window);
	gtk_ui_manager_insert_action_group (window->priv->ui_manager, window->priv->ui_actions, 0);

	if (!gtk_ui_manager_add_ui_from_string (window->priv->ui_manager, UI_DEFINITION, -1, NULL)) {
		g_assert ("No se pudo cargar la UI");
	}

	menubar = gtk_ui_manager_get_widget (window->priv->ui_manager, "/MainMenu");
	g_assert (GTK_IS_WIDGET (menubar));
	gtk_box_pack_start (GTK_BOX (window->priv->box), menubar, FALSE, FALSE, 0);
	gtk_widget_show (menubar);

	gtk_window_add_accel_group (GTK_WINDOW (window),
				    gtk_ui_manager_get_accel_group (window->priv->ui_manager));

	window->priv->graph = gs_graph_new ();
	gtk_widget_show (window->priv->graph);

	window->priv->hpaned = gtk_hpaned_new ();
	gtk_paned_pack1 (GTK_PANED (window->priv->hpaned), window->priv->graph, TRUE, FALSE);

	window->priv->plot_list_panel = gs_plot_list_panel_new (GS_WINDOW (window));
	gtk_paned_pack2 (GTK_PANED (window->priv->hpaned), window->priv->plot_list_panel, TRUE, FALSE);
	gtk_widget_show (window->priv->plot_list_panel);
	gtk_widget_show (window->priv->hpaned);

	gtk_box_pack_start (GTK_BOX (window->priv->box), window->priv->hpaned, TRUE, TRUE, 0);

	window->priv->statusbar = gtk_statusbar_new ();
	gtk_box_pack_end (GTK_BOX (window->priv->box), window->priv->statusbar, FALSE, FALSE, 0);
	gtk_widget_show (window->priv->statusbar);

}

static void
gs_window_init (GsWindow *self)
{
	self->priv = GS_WINDOW_GET_PRIVATE (self);

	gs_window_build_ui (self);
}

static void
gs_window_class_init (GsWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = gs_window_finalize;

	g_type_class_add_private (object_class, sizeof (GsWindowPrivate));
}

GtkWidget *
gs_window_new (void)
{
	return GTK_WIDGET (g_object_new (GS_TYPE_WINDOW, NULL));
}

GsGraph *
gs_window_get_graph (GsWindow *window)
{
	g_return_val_if_fail (GS_IS_WINDOW (window), NULL);

	return GS_GRAPH (window->priv->graph);
}
