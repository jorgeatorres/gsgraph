#include <glib.h>
#include <gdk/gdk.h>
#include "gs-misc.h"

#ifndef __GS_PLOT_PRIVATE_H__
#define __GS_PLOT_PRIVATE_H__

struct _GsPlotPrivate
{
	gchar *name;
	GdkColor color;
};

#endif
