/*
 * gsdebug.h v0.1
 * Copyright (C) 2006 Alexandre Pereira Bueno <alpebu@yahoo.com.br>
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

#ifndef __GSDEBUG_H__
#define __GSDEBUG_H__

#include <glib.h>

G_BEGIN_DECLS

#define ENABLE_DEBUGGING

#ifdef ENABLE_DEBUGGING
#define LOG(msg, args...) (g_log (G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "[%s] " msg, G_STRLOC, ##args))
#else
#define LOG(...)
#endif

G_END_DECLS

#endif /* __GSDEBUG_H__ */
