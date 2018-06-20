/* Copyright 1995-1996,1999-2001,2006,2008,2010,2018
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




#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>

#include "gsubr.h"
#include "numbers.h"
#include "strings.h"

#include "version.h"




#define SCM_TMP_MACRO_MKSTR(x) #x

/* Return a Scheme string containing Guile's major version number.  */

SCM_DEFINE (scm_major_version, "major-version", 0, 0, 0, 
            (),
            "Return a string containing Guile's major version number.\n"
            "E.g., the 1 in \"1.6.5\".")
#define FUNC_NAME s_scm_major_version
{
  return scm_number_to_string (scm_from_int (SCM_MAJOR_VERSION),
                               scm_from_int (10));
}
#undef FUNC_NAME

/* Return a Scheme string containing Guile's minor version number.  */

SCM_DEFINE (scm_minor_version, "minor-version", 0, 0, 0, 
            (),
            "Return a string containing Guile's minor version number.\n"
            "E.g., the 6 in \"1.6.5\".")
#define FUNC_NAME s_scm_minor_version
{
  return scm_number_to_string (scm_from_int (SCM_MINOR_VERSION),
                               scm_from_int (10));
}
#undef FUNC_NAME

/* Return a Scheme string containing Guile's micro version number.  */

SCM_DEFINE (scm_micro_version, "micro-version", 0, 0, 0, 
            (),
            "Return a string containing Guile's micro version number.\n"
            "E.g., the 5 in \"1.6.5\".")
#define FUNC_NAME s_scm_micro_version
{
  return scm_number_to_string (scm_from_int (SCM_MICRO_VERSION),
                               scm_from_int (10));
}
#undef FUNC_NAME

/* Return a Scheme string containing Guile's complete version.  */

SCM_DEFINE (scm_version, "version", 0, 0, 0, 
            (),
	    "@deffnx {Scheme Procedure} major-version\n"
	    "@deffnx {Scheme Procedure} minor-version\n"
	    "@deffnx {Scheme Procedure} micro-version\n"
	    "Return a string describing Guile's version number, or its major, minor\n"
	    "or micro version number, respectively.\n\n"
	    "@lisp\n"
	    "(version) @result{} \"1.6.0\"\n"
	    "(major-version) @result{} \"1\"\n"
	    "(minor-version) @result{} \"6\"\n"
	    "(micro-version) @result{} \"0\"\n"
	    "@end lisp")
#define FUNC_NAME s_scm_version
{
  return scm_from_locale_string (PACKAGE_VERSION);
}
#undef FUNC_NAME

/* Return a Scheme string containing Guile's effective version.  */

SCM_DEFINE (scm_effective_version, "effective-version", 0, 0, 0, 
            (),
	    "Return a string describing Guile's effective version number.\n"
	    "@lisp\n"
	    "(version) @result{} \"1.6.0\"\n"
	    "(effective-version) @result{} \"1.6\"\n"
	    "(major-version) @result{} \"1\"\n"
	    "(minor-version) @result{} \"6\"\n"
	    "(micro-version) @result{} \"0\"\n"
	    "@end lisp")
#define FUNC_NAME s_scm_effective_version
{
  return scm_from_locale_string (SCM_EFFECTIVE_VERSION);
}
#undef FUNC_NAME




void
scm_init_version ()
{
#include "version.x"
}
