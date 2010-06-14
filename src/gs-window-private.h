#include "gs-window.h"

G_BEGIN_DECLS

struct _GsWindowPrivate {
	GtkWidget *graph;

	GtkUIManager *ui_manager;
	GtkActionGroup *ui_actions;

	GtkWidget *box;
	GtkWidget *statusbar;

	GtkWidget *hpaned;
	GtkWidget *plot_list_panel;
};

G_END_DECLS
