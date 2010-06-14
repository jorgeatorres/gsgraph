#include "gs-window.h"

int main (int argc, char **argv)
{
	GtkWidget *window;

	gtk_init (&argc, &argv);
	g_set_application_name ("GsGraph");

	window = gs_window_new ();
	gtk_widget_show (window);

	g_signal_connect (G_OBJECT (window), "delete-event", G_CALLBACK (gtk_main_quit), NULL);

	gtk_main ();

}
