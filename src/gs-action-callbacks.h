/*
 * gs-ui-callbacks.h
 */

#ifndef __GS_ACTION_CALLBACKS_H__
#define __GS_ACTION_CALLBACKS_H__

#include <gtk/gtk.h>
#include "gs-window.h"

G_BEGIN_DECLS

void _gs_cb_view_show_grid (GtkToggleAction *action, GsWindow *window);
void _gs_cb_view_show_plots_panel (GtkToggleAction *action, GsWindow *window);

void _gs_cb_help_about (GtkAction *action, GsWindow *window);


G_END_DECLS

#endif /* __GS_ACTION_CALLBACKS_H__ */
 
