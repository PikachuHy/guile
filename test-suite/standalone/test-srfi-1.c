/* Copyright (C) 2010 Free Software Foundation, Inc.
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

/* Exercise the compatibility layer of `libguile-srfi-srfi-1'.  */

#ifndef HAVE_CONFIG_H
# include <config.h>
#endif

#include <libguile.h>
#include <srfi/srfi-1.h>

#include <stdlib.h>

static void
failure (const char *proc, SCM result)
{
  scm_simple_format (scm_current_error_port (),
		     scm_from_locale_string ("`~S' failed: ~S~%"),
		     scm_list_2 (scm_from_locale_symbol (proc), result));
}

static void *
tests (void *data)
{
  SCM times, negative_p, lst, result;

  scm_init_srfi_1 ();

  times = SCM_VARIABLE_REF (scm_c_lookup ("*"));
  lst = scm_list_3 (scm_from_int (1), scm_from_int (2), scm_from_int (3));

  /* (fold * 1 '(1 2 3) '(1 2 3)) */
  result = scm_srfi1_fold (times, scm_from_int (1), lst, scm_list_1 (lst));

  if (scm_to_int (result) == 36)
    {
      negative_p = SCM_VARIABLE_REF (scm_c_lookup ("negative?"));
      result = scm_srfi1_break (negative_p,
				scm_list_3 (scm_from_int (1),
					    scm_from_int (2),
					    scm_from_int (-1)));

      if (SCM_VALUESP (result))
	/* There's no API to access the values, so assume this is OK.  */
	* (int *) data = EXIT_SUCCESS;
      else
	{
	  failure ("break", result);
	  * (int *) data = EXIT_FAILURE;
	}
    }
  else
    {
      failure ("fold", result);
      * (int *) data = EXIT_FAILURE;
    }

  return data;
}


int
main (int argc, char *argv[])
{
  int ret;

  scm_with_guile (tests, &ret);

  return ret;
}