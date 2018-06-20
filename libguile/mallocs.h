#ifndef SCM_MALLOCS_H
#define SCM_MALLOCS_H

/* Copyright 1995,2000,2006,2008,2018
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


SCM_API scm_t_bits scm_tc16_malloc;

#define SCM_MALLOCP(X)  (SCM_SMOB_PREDICATE (scm_tc16_malloc, (X)))
#define SCM_MALLOCDATA(obj) ((char *) SCM_SMOB_DATA (obj))
#define SCM_SETMALLOCDATA(obj, val) (SCM_SET_SMOB_DATA ((obj), (val)))



SCM_API SCM scm_malloc_obj (size_t n);
SCM_INTERNAL void scm_init_mallocs (void);

#endif  /* SCM_MALLOCS_H */
