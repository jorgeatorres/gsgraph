/*
 * gs-misc.c
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

#include <glib.h>

#include "gs-misc.h"

GsPoint *
gs_point_copy (const GsPoint *point)
{
	GsPoint *new_point = g_new (GsPoint, 1);
	new_point->x = point->x;
	new_point->y = point->y;
	
	return new_point;
}

void
gs_point_to_pixel (GsGraphTransform *t, GsPoint *src, GsPoint *dest)
{
	g_return_if_fail (t != NULL);

	t->point_to_pixel (src, dest, t->data);
}

void
gs_pixel_to_point (GsGraphTransform *t, GsPoint *src, GsPoint *dest)
{
	g_return_if_fail (t != NULL);

	t->pixel_to_point (src, dest, t->data);
}

void
gs_point_to_pixel_values (GsGraphTransform *t, gdouble x, gdouble y, gdouble *dest_x, gdouble *dest_y)
{
	g_return_if_fail (t != NULL);

	GsPoint src, dest;
	src.x = x;
	src.y = y;

	gs_point_to_pixel (t, &src, &dest);

	if (dest_x != NULL) {
		*dest_x = dest.x;
	}

	if (dest_y != NULL) {
		*dest_y = dest.y;
	}
	
}

void
gs_pixel_to_point_values (GsGraphTransform *t, gdouble x, gdouble y, gdouble *dest_x, gdouble *dest_y)
{
	g_return_if_fail (t != NULL);

	GsPoint src, dest;
	src.x = x;
	src.y = y;

	gs_pixel_to_point (t, &src, &dest);

	if (dest_x != NULL) {
		*dest_x = dest.x;
	}

	if (dest_y != NULL) {
		*dest_y = dest.y;
	}

}
