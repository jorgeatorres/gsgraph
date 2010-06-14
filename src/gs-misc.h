/*
 * gs-misc.h
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

#ifndef __GS_MISC_H__ 
#define __GS_MISC_H__

G_BEGIN_DECLS

#define GS_COLOR_CAIRO(x) x.r/255.0, x.g/255.0, x.b/255.0
#define GS_COLOR_CAIROA(x) GS_COLOR_CAIRO(x), x.a

typedef struct _GsPoint GsPoint;
typedef struct _GsColor GsColor;
typedef struct _GsRectangle GsRectangle;

typedef struct _GsGraphTransform GsGraphTransform;

struct _GsPoint
{
	gdouble x;
	gdouble y;
};

struct _GsColor
{
	gdouble r;
	gdouble g;
	gdouble b;
	gdouble a;
};

struct _GsRectangle
{
	gdouble x1;
	gdouble y1;

	gdouble x2;
	gdouble y2;
};

struct _GsGraphTransform
{
	/* <opaque> */
	gpointer data;
	void (*point_to_pixel) (GsPoint *src, GsPoint *dest, gpointer data);
	void (*pixel_to_point) (GsPoint *src, GsPoint *dest, gpointer data);
};

GsPoint	*gs_point_copy (const GsPoint *point);

void	gs_point_to_pixel (GsGraphTransform *t, GsPoint *src, GsPoint *dest);
void	gs_pixel_to_point (GsGraphTransform *t, GsPoint *src, GsPoint *dest);
void	gs_point_to_pixel_values (GsGraphTransform *t, gdouble x, gdouble y, gdouble *dest_x, gdouble *dest_y);
void	gs_pixel_to_point_values (GsGraphTransform *t, gdouble x, gdouble y, gdouble *dest_x, gdouble *dest_y);

G_END_DECLS

#endif /* __GS_MISC_H__ */
