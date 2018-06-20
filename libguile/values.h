#ifndef SCM_VALUES_H
#define SCM_VALUES_H

/* Copyright 2000-2001,2006,2008,2012,2018
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



#include "libguile/struct.h"

SCM_API SCM scm_values_vtable;

#define SCM_VALUESP(x) (SCM_STRUCTP (x)\
                        && scm_is_eq (scm_struct_vtable (x), scm_values_vtable))

SCM_INTERNAL void scm_i_extract_values_2 (SCM obj, SCM *p1, SCM *p2);

SCM_API SCM scm_values (SCM args);
SCM_API SCM scm_c_values (SCM *base, size_t n);
SCM_API size_t scm_c_nvalues (SCM obj);
SCM_API SCM scm_c_value_ref (SCM obj, size_t idx);
SCM_INTERNAL void scm_init_values (void);

#endif  /* SCM_VALUES_H */
