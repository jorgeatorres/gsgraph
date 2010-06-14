#include <math.h>
#include <gtk/gtk.h>
#include <matheval.h>

#include "gs-graph.h"
#include "gs-function-plot.h"
#include "gs-dataset-plot.h"

/*
 * Example functions
 */
gdouble func5 (gdouble x) { return (gdouble) (pow(x,2)+(3*pow(x,4))-(2*pow(x,5))+3); } 

/* Main widget */
GtkWidget *window, *graph;
GtkWidget *funcentry, *funccolor;

static void
graph_it (GtkButton *button, gpointer data)
{
	GdkColor color;
	const gchar *funcstr = gtk_entry_get_text (GTK_ENTRY (funcentry));
	gtk_color_button_get_color (GTK_COLOR_BUTTON (funccolor), &color);

	GsPlot *plot = gs_function_plot_new ();
	gs_function_plot_set_formula (GS_FUNCTION_PLOT (plot), funcstr);
	gs_plot_set_name (plot, funcstr);
	gs_plot_set_color (plot, &color);

	gtk_entry_set_text (GTK_ENTRY (funcentry), "");

	gs_graph_attach_plot (GS_GRAPH (graph), plot);
}

static void
toggle_background_cb (GtkToggleButton *button)
{
	gs_graph_set_paint_background (GS_GRAPH (graph), gtk_toggle_button_get_active (button));
}

static void
toggle_grid_cb (GtkToggleButton *button)
{
	gs_graph_set_show_grid (GS_GRAPH (graph), gtk_toggle_button_get_active (button));
}

static void
toggle_dotted_grid_cb (GtkToggleButton *button)
{
	gs_graph_set_grid_dotted (GS_GRAPH (graph), gtk_toggle_button_get_active (button));
}

static void
toggle_horizontal_axis_cb (GtkToggleButton *button)
{
	gs_graph_set_show_horizontal_axis (GS_GRAPH (graph), gtk_toggle_button_get_active (button));
}

static void
toggle_vertical_axis_cb (GtkToggleButton *button)
{
	gs_graph_set_show_vertical_axis (GS_GRAPH (graph), gtk_toggle_button_get_active (button));
}

static void
toggle_ticks_cb (GtkToggleButton *button)
{
	gs_graph_set_show_ticks (GS_GRAPH (graph), gtk_toggle_button_get_active (button));
}

static void
tick_length_cb (GtkSpinButton *button)
{
	gs_graph_set_tick_length (GS_GRAPH (graph), gtk_spin_button_get_value_as_int (button));
}

static void
grid_dx_cb (GtkSpinButton *button)
{
	gs_graph_set_grid_horizontal_spacing (GS_GRAPH (graph), gtk_spin_button_get_value (button));
}

static void
grid_dy_cb (GtkSpinButton *button)
{
	gs_graph_set_grid_vertical_spacing (GS_GRAPH (graph), gtk_spin_button_get_value (button));
}

static void
background_cb_cb (GtkColorButton *button)
{
	GdkColor color;
	gtk_color_button_get_color (button, &color);

	gs_graph_set_background_color (GS_GRAPH (graph), &color);
}

static void
grid_cb_cb (GtkColorButton *button)
{
	GdkColor color;
	gtk_color_button_get_color (button, &color);

	gs_graph_set_grid_color (GS_GRAPH (graph), &color);
}

static void
axis_cb_cb (GtkColorButton *button)
{
	GdkColor color;
	gtk_color_button_get_color (button, &color);

	gs_graph_set_axes_color (GS_GRAPH (graph), &color);
}

/* This code was (almost) automagically created by glade :) */
static GtkWidget *
create_test_options_dialog (void)
{
  GtkWidget *dialog2;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *expander1;
  GtkWidget *label1;
  GtkWidget *expander2;
  GtkWidget *vbox2;
  GtkWidget *toggle_horizontal_axis;
  GtkWidget *toggle_vertical_axis;
  GtkWidget *hbox2;
  GtkWidget *toggle_ticks;
  GtkWidget *hbox3;
  GtkWidget *label4;
  GtkWidget *tick_length;
  GtkWidget *hbox1;
  GtkWidget *toggle_grid;
  GtkWidget *toggle_dotted_grid;
  GtkWidget *hbox4;
  GtkWidget *hbox5;
  GtkWidget *label5;
  GtkWidget *grid_dx;
  GtkWidget *hbox6;
  GtkWidget *label6;
  GtkWidget *grid_dy;
  GtkWidget *label2;
  GtkWidget *expander3;
  GtkWidget *vbox3;
  GtkWidget *toggle_background;
  GtkWidget *table1;
  GtkWidget *label7;
  GtkWidget *label8;
  GtkWidget *label9;
  GtkWidget *background_cb;
  GtkWidget *grid_cb;
  GtkWidget *axes_cb;
  GtkWidget *label3;
  GtkWidget *dialog_action_area1;

  dialog2 = gtk_dialog_new ();
  gtk_window_set_transient_for (GTK_WINDOW (dialog2), GTK_WINDOW (window));
  gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog2), TRUE);
  g_signal_connect (G_OBJECT (dialog2), "delete-event", G_CALLBACK (gtk_true), NULL);
  gtk_window_set_title (GTK_WINDOW (dialog2), ("Test Options"));
  gtk_window_set_type_hint (GTK_WINDOW (dialog2), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (dialog2)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox1 = gtk_vbox_new (TRUE, 5);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);
/*
  expander1 = gtk_expander_new (NULL);
  gtk_widget_show (expander1);
  gtk_box_pack_start (GTK_BOX (vbox1), expander1, TRUE, TRUE, 0);

  label1 = gtk_label_new (("<b>Plots</b>"));
  gtk_widget_show (label1);
  gtk_expander_set_label_widget (GTK_EXPANDER (expander1), label1);
  gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);*/

  expander2 = gtk_expander_new (NULL);
  gtk_widget_show (expander2);
  gtk_box_pack_start (GTK_BOX (vbox1), expander2, TRUE, TRUE, 0);
  gtk_expander_set_expanded (GTK_EXPANDER (expander2), TRUE);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (expander2), vbox2);

  toggle_horizontal_axis = gtk_check_button_new_with_mnemonic (("Show horizontal axis"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle_horizontal_axis), TRUE);
  g_signal_connect (G_OBJECT (toggle_horizontal_axis), "toggled", G_CALLBACK (toggle_horizontal_axis_cb), NULL);
  gtk_widget_show (toggle_horizontal_axis);
  gtk_box_pack_start (GTK_BOX (vbox2), toggle_horizontal_axis, FALSE, FALSE, 0);

  toggle_vertical_axis = gtk_check_button_new_with_mnemonic (("Show vertical axis"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle_vertical_axis), TRUE);
  g_signal_connect (G_OBJECT (toggle_vertical_axis), "toggled", G_CALLBACK (toggle_vertical_axis_cb), NULL);
  gtk_widget_show (toggle_vertical_axis);
  gtk_box_pack_start (GTK_BOX (vbox2), toggle_vertical_axis, FALSE, FALSE, 0);

  hbox2 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox2, FALSE, FALSE, 0);

  toggle_ticks = gtk_check_button_new_with_mnemonic (("Show ticks"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle_ticks), TRUE);
  g_signal_connect (G_OBJECT (toggle_ticks), "toggled", G_CALLBACK (toggle_ticks_cb), NULL);
  gtk_widget_show (toggle_ticks);
  gtk_box_pack_start (GTK_BOX (hbox2), toggle_ticks, FALSE, FALSE, 0);

  hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox3);
  gtk_box_pack_start (GTK_BOX (hbox2), hbox3, TRUE, TRUE, 0);

  label4 = gtk_label_new (("Tick length (px): "));
  gtk_widget_show (label4);
  gtk_box_pack_start (GTK_BOX (hbox3), label4, FALSE, FALSE, 0);

  tick_length = gtk_spin_button_new_with_range (2.0, 30.0, 1.0);
  gtk_spin_button_set_digits (GTK_SPIN_BUTTON (tick_length), 0);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (tick_length), 4.0);
  g_signal_connect (G_OBJECT (tick_length), "value-changed", G_CALLBACK (tick_length_cb), NULL);
  gtk_widget_show (tick_length);
  gtk_box_pack_start (GTK_BOX (hbox3), tick_length, FALSE, TRUE, 0);

  hbox1 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox1, FALSE, TRUE, 0);

  toggle_grid = gtk_check_button_new_with_mnemonic (("Show grid"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle_grid), TRUE);
  g_signal_connect (G_OBJECT (toggle_grid), "toggled", G_CALLBACK (toggle_grid_cb), NULL);
  gtk_widget_show (toggle_grid);
  gtk_box_pack_start (GTK_BOX (hbox1), toggle_grid, FALSE, FALSE, 0);

  toggle_dotted_grid = gtk_check_button_new_with_mnemonic (("Dotted grid"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle_dotted_grid), FALSE);
  g_signal_connect (G_OBJECT (toggle_dotted_grid), "toggled", G_CALLBACK (toggle_dotted_grid_cb), NULL);
  gtk_widget_show (toggle_dotted_grid);
  gtk_box_pack_start (GTK_BOX (hbox1), toggle_dotted_grid, FALSE, FALSE, 0);

  hbox4 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox4, FALSE, TRUE, 0);

  hbox5 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox5);
  gtk_box_pack_start (GTK_BOX (hbox4), hbox5, TRUE, TRUE, 0);

  label5 = gtk_label_new (("Grid horiz. spacing: "));
  gtk_widget_show (label5);
  gtk_box_pack_start (GTK_BOX (hbox5), label5, FALSE, FALSE, 0);

  grid_dx = gtk_spin_button_new_with_range (0.1, G_MAXDOUBLE, 0.1);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (grid_dx), 1.0);
  gtk_spin_button_set_digits (GTK_SPIN_BUTTON (grid_dx), 1);
  g_signal_connect (G_OBJECT (grid_dx), "value-changed", G_CALLBACK (grid_dx_cb), NULL);
  gtk_widget_show (grid_dx);
  gtk_box_pack_start (GTK_BOX (hbox5), grid_dx, TRUE, TRUE, 0);

  hbox6 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox6);
  gtk_box_pack_start (GTK_BOX (hbox4), hbox6, TRUE, TRUE, 0);

  label6 = gtk_label_new (("Grid vert. spacing: "));
  gtk_widget_show (label6);
  gtk_box_pack_start (GTK_BOX (hbox6), label6, FALSE, FALSE, 0);

  grid_dy = gtk_spin_button_new_with_range (0.1, G_MAXDOUBLE, 0.1);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (grid_dy), 1.0);
  gtk_spin_button_set_digits (GTK_SPIN_BUTTON (grid_dy), 1);
  g_signal_connect (G_OBJECT (grid_dy), "value-changed", G_CALLBACK (grid_dy_cb), NULL);
  gtk_widget_show (grid_dy);
  gtk_box_pack_start (GTK_BOX (hbox6), grid_dy, TRUE, TRUE, 0);

  label2 = gtk_label_new (("<b>Grid, ticks and axes</b>"));
  gtk_widget_show (label2);
  gtk_expander_set_label_widget (GTK_EXPANDER (expander2), label2);
  gtk_label_set_use_markup (GTK_LABEL (label2), TRUE);

  expander3 = gtk_expander_new (NULL);
  gtk_widget_show (expander3);
  gtk_box_pack_start (GTK_BOX (vbox1), expander3, TRUE, TRUE, 0);
  gtk_expander_set_expanded (GTK_EXPANDER (expander3), TRUE);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox3);
  gtk_container_add (GTK_CONTAINER (expander3), vbox3);

  toggle_background = gtk_check_button_new_with_mnemonic (("Paint background"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle_background), TRUE);
  g_signal_connect (G_OBJECT (toggle_background), "toggled", G_CALLBACK (toggle_background_cb), NULL);
  gtk_widget_show (toggle_background);
  gtk_box_pack_start (GTK_BOX (vbox3), toggle_background, FALSE, FALSE, 0);

  table1 = gtk_table_new (3, 2, FALSE);
  gtk_widget_show (table1);
  gtk_box_pack_start (GTK_BOX (vbox3), table1, TRUE, TRUE, 0);

  label7 = gtk_label_new (("Background color: "));
  gtk_widget_show (label7);
  gtk_table_attach (GTK_TABLE (table1), label7, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label7), 0, 0.5);

  label8 = gtk_label_new (("Grid color: "));
  gtk_widget_show (label8);
  gtk_table_attach (GTK_TABLE (table1), label8, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label8), 0, 0.5);

  label9 = gtk_label_new (("Axes color: "));
  gtk_widget_show (label9);
  gtk_table_attach (GTK_TABLE (table1), label9, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label9), 0, 0.5);

  background_cb = gtk_color_button_new ();
  g_signal_connect (G_OBJECT (background_cb), "color-set", G_CALLBACK (background_cb_cb), NULL);
  gtk_widget_show (background_cb);
  gtk_table_attach (GTK_TABLE (table1), background_cb, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  grid_cb = gtk_color_button_new ();
  g_signal_connect (G_OBJECT (grid_cb), "color-set", G_CALLBACK (grid_cb_cb), NULL);
  gtk_widget_show (grid_cb);
  gtk_table_attach (GTK_TABLE (table1), grid_cb, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  axes_cb = gtk_color_button_new ();
  g_signal_connect (G_OBJECT (axes_cb), "color-set", G_CALLBACK (axis_cb_cb), NULL);
  gtk_widget_show (axes_cb);
  gtk_table_attach (GTK_TABLE (table1), axes_cb, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label3 = gtk_label_new (("<b>Style/Color Options</b>"));
  gtk_widget_show (label3);
  gtk_expander_set_label_widget (GTK_EXPANDER (expander3), label3);
  gtk_label_set_use_markup (GTK_LABEL (label3), TRUE);

  dialog_action_area1 = GTK_DIALOG (dialog2)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  gtk_widget_show_all (dialog2);

  return dialog2;
}



int main (int argc, char *argv[])
{
	GdkColor color;

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "GsGraph Test");
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	g_signal_connect (G_OBJECT (window), "destroy", (GCallback) gtk_main_quit, NULL);

	graph = gs_graph_new ();

	GtkWidget *box;
	box = gtk_vbox_new (FALSE, 5);
	gtk_container_add (GTK_CONTAINER (box), graph);

	/* Function textbox */
	GtkWidget *add_btn, *func_txt, *fhbox, *fcolor;
	funccolor = gtk_color_button_new ();
	fhbox = gtk_hbox_new (FALSE, 5);
	add_btn = gtk_button_new_with_label ("Graph It!");
	funcentry = gtk_entry_new ();
	g_signal_connect (G_OBJECT (add_btn), "clicked", G_CALLBACK (graph_it), NULL);

	gtk_container_add (GTK_CONTAINER (fhbox), funcentry);
	gtk_container_add (GTK_CONTAINER (fhbox), funccolor);
	gtk_container_add (GTK_CONTAINER (fhbox), add_btn);
	gtk_container_add (GTK_CONTAINER (box), fhbox);
	gtk_container_add (GTK_CONTAINER (window), box);

	// XXX - This is leaking...
	gs_graph_attach_plot (GS_GRAPH (graph), gs_function_plot_new_with_formula ("sin(x)"));
	gs_graph_attach_plot (GS_GRAPH (graph), gs_function_plot_new_with_formula ("x*x"));
	gs_graph_attach_plot (GS_GRAPH (graph), gs_function_plot_new_with_formula ("x^3"));

	/* Dataset plot */
	gdouble x[] = { -3.0, 3.0, 2.5 };
	gdouble y[] = { -3.0, 3.0, 1.0 };

	gs_graph_attach_plot (GS_GRAPH (graph), gs_dataset_plot_new_with_data (x, y, 3));

	gtk_widget_show_all (window);

	create_test_options_dialog ();
	
	gtk_main ();
	
	return 0;
}
