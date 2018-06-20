#ifndef SCM_DEBUG_H
#define SCM_DEBUG_H

/* Copyright 1995-1996,1998-2002,2004,2008-2013,2015,2018
     Free Software Foundation, Inc.

   This file is part of Guile.

   Guile is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Guile is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
   License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with Guile.  If not, see
   <https://www.gnu.org/licenses/>.  */



#include "libguile/options.h"



SCM_API SCM scm_local_eval (SCM exp, SCM env);

SCM_API SCM scm_reverse_lookup (SCM env, SCM data);
SCM_API SCM scm_debug_options (SCM setting);

SCM_INTERNAL void scm_init_debug (void);

#ifdef GUILE_DEBUG
SCM_API SCM scm_debug_hang (SCM obj);
#endif /*GUILE_DEBUG*/

#endif  /* SCM_DEBUG_H */
