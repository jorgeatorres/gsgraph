#include "gs-debug.h"
#include "gs-window-private.h"
#include "gs-action-callbacks.h"

void _gs_cb_view_show_plots_panel (GtkToggleAction *action, GsWindow *window)
{
	if (gtk_toggle_action_get_active (action)) {
		gtk_widget_show (window->priv->plot_list_panel);	
	} else {
		gtk_widget_hide (window->priv->plot_list_panel);
	}
}

void _gs_cb_view_show_grid (GtkToggleAction *action, GsWindow *window)
{
	gs_graph_set_show_grid (GS_GRAPH (window->priv->graph),
				gtk_toggle_action_get_active (action));
}


static void dialog_hook (GtkAboutDialog *about, const gchar *link, gpointer data) { }

/* HELP */
void _gs_cb_help_about (GtkAction *action, GsWindow *window)
{
	GdkPixbuf *logo;

	static const gchar * const authors[] = {
			"Jorge Torres <jorge@0xfee1dead.org>",
			NULL
	};

	static const gchar copyright[] = \
		"Copyright \xc2\xa9 2007 Jorge Torres";

	logo = gdk_pixbuf_new_from_file ("gsgraph-logo.png", NULL);

	gtk_about_dialog_set_url_hook (dialog_hook, NULL, NULL);

	gtk_show_about_dialog (GTK_WINDOW (window),
			       "authors", authors,
			       "copyright", copyright,
			       "logo", logo,
			       "website", "http://0xfee1dead.org/gsgraph",
			       "comments", "\"Eso no fue un pico, fue una mala metida de dedo\"\nDaPa",
			       "version", "0.1",
			       NULL);

	if (logo != NULL)
		g_object_unref (logo);
}
