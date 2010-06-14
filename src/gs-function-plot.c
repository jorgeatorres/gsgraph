/*
 * gs-plot-function.c
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

#include <matheval.h>
#include <math.h>

#include "gs-debug.h"
#include "gs-plot-private.h"
#include "gs-function-plot.h"

struct _GsFunctionPlotPrivate
{
	void *matheval_c;
};

G_DEFINE_TYPE (GsFunctionPlot, gs_function_plot, GS_TYPE_PLOT);

/*
 * TODO - Como graficar de manera que el "path" que se forma al unir los puntos
 * concuerde con la gráfica. Por ejemplo, la función tan(x) presenta asíntotas verticales
 * en los múltiplos impares de pi/2.
 * Entonces se tiene un punto de la forma (a,f(a)) unido a (b,f(b)) donde a < n*pi/2 < b, y 
 * f(a) >>>> f(b)... entonces se genera una linea que atraviesa la que seria la asintota horizontal.
 */
static void
gs_function_plot_draw (GsPlot *plot, GsGraphTransform *t, GsRectangle *v, cairo_t *cr)
{
	GsPoint point, d_point, d_prev_point;

	if (GS_FUNCTION_PLOT (plot)->priv->matheval_c == NULL) {
		return;
	}

	d_prev_point. x = -1;
	d_prev_point.y = -1;

	cairo_save (cr);
	gdk_cairo_set_source_color (cr, &(plot->priv->color));
//	cairo_set_line_join (cr, CAIRO_LINE_JOIN_BEVEL);

	for (point.x = v->x1; point.x <= v->x2; point.x += 0.05) {
		point.y = evaluator_evaluate_x (GS_FUNCTION_PLOT (plot)->priv->matheval_c,
						point.x);

		gs_point_to_pixel (t, &point, &d_point);

		if (d_prev_point.x > -1 && d_prev_point.y > -1) {
/*			cairo_move_to (cr, d_prev_point.x, d_prev_point.y);
			cairo_line_to (cr, d_point.x, d_point.y);
			cairo_stroke (cr);*/
			cairo_arc (cr, d_point.x, d_point.y, 1., 0., M_PI * 2);
			cairo_fill (cr);
		}

		d_prev_point.x = d_point.x;
		d_prev_point.y = d_point.y;
	}

	cairo_restore (cr);
}

static void
gs_function_plot_finalize (GObject *object)
{
	GsFunctionPlot *plot = GS_FUNCTION_PLOT (object);

	if (plot->priv->matheval_c != NULL) {
		evaluator_destroy (plot->priv->matheval_c);
	}

	G_OBJECT_CLASS (gs_function_plot_parent_class)->finalize (object);
}

static void
gs_function_plot_init (GsFunctionPlot *self)
{
	self->priv = g_new0 (GsFunctionPlotPrivate, 1);
	self->priv->matheval_c = NULL;
}

static void
gs_function_plot_class_init (GsFunctionPlotClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GsPlotClass *plot_class = GS_PLOT_CLASS (klass);

	object_class->finalize = gs_function_plot_finalize;
	plot_class->draw_plot = gs_function_plot_draw;
}

GsPlot *
gs_function_plot_new (void)
{
	GsPlot *ret = GS_PLOT (g_object_new (GS_TYPE_FUNCTION_PLOT,
					     NULL));

	return ret;
}

GsPlot *
gs_function_plot_new_with_formula (const gchar *formula)
{
	GsPlot *ret = gs_function_plot_new ();
	gs_function_plot_set_formula (GS_FUNCTION_PLOT (ret), formula);

	return ret;
}

/*  TODO - Revisar que la formula sea correcta:
 *  No tenga caracteres diferentes a una constante permitida y x.
 *  Revisar que la formula tenga sentido para libmatheval... si no es el caso, explotar!
 */
void
gs_function_plot_set_formula (GsFunctionPlot *plot, const gchar *formula)
{
	g_return_if_fail (GS_IS_FUNCTION_PLOT (plot));

	void *f = evaluator_create ((char *) formula);

	g_assert (f != NULL);

/*	if (f == NULL) {
		return FALSE;
	}*/

	if (plot->priv->matheval_c != NULL) {
		evaluator_destroy (plot->priv->matheval_c);
	}

	plot->priv->matheval_c = f;

	g_signal_emit_by_name (G_OBJECT (plot), "plot-changed");
}

gchar *
gs_function_plot_get_formula (GsFunctionPlot *plot)
{
	g_return_val_if_fail (GS_IS_FUNCTION_PLOT (plot), NULL);
	
	if (plot->priv->matheval_c == NULL) {
		return NULL;
	}

	return g_strdup ((gchar *) evaluator_get_string (plot->priv->matheval_c));
}
