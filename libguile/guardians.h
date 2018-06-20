#ifndef SCM_GUARDIANS_H
#define SCM_GUARDIANS_H

/* Copyright 1998,2000-2001,2006,2008,2018
 *   Free Software Foundation, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */



#include "libguile/scm.h"

SCM_API SCM scm_make_guardian (void);

SCM_INTERNAL void scm_i_init_guardians_for_gc (void);
SCM_INTERNAL void scm_i_identify_inaccessible_guardeds (void);
SCM_INTERNAL int scm_i_mark_inaccessible_guardeds (void);

SCM_INTERNAL void scm_init_guardians (void);

#endif  /* SCM_GUARDIANS_H */
