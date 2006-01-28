/*  dctrl-tools - Debian control file inspection tools
    Copyright (C) 1999 Antti-Juhani Kaijanaho

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef MSG_H__
#define MSG_H__

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "i18n.h"

static inline
void fail(void) { exit(2); }

/* log levels */
#define L_FATAL 3
#define L_IMPORTANT 2
#define L_INFORMATIONAL 1
#define L_DEBUG 0

#define PROGNAME_MAXLEN 64

#if !defined(NDEBUG) && !defined(TEST_NODEBUG) && defined(ENABLE_L_DEBUG)
#  define INCLUDE_DEBUG_MSGS
#endif

#ifdef MSG_C__
int loglevel = L_IMPORTANT;
char progname [PROGNAME_MAXLEN];
#endif

inline static int
do_msg(int severity)
{
#ifndef MSG_C__
  extern int loglevel;
#endif

#if defined(TEST_NODEBUG)
  if (severity == L_DEBUG) {
	  message (L_FATAL, _("I'm broken - please report this bug."), 0, 0);
	  abort ();
  }
#endif
  
  return severity >= loglevel;
}

inline static void
line_message (int severity, const char * s, const char * fname, int line)
{
#ifndef MSG_C__
	extern const char progname [PROGNAME_MAXLEN];
	extern bool errors;
#endif
	if (do_msg(severity)) {
      
		if (fname == 0) {
			fprintf (stderr,  "%s: %s.\n", progname, s);
		} else {
			if (line > 0) {
				fprintf (stderr, "%s: %s: %i: %s.\n", progname, fname, line, s);
			} else {
				fprintf (stderr, "%s: %s: %s.\n", progname, fname, s);
			}
		}
		if (severity >= L_IMPORTANT) errors = true;
	}
}

inline static void
message (int severity, const char * s, const char * fname)
{
  line_message (severity, s, fname, 0);
}

#ifndef MSG_C__
#undef PROGNAME_MAXLEN
#endif

inline static void
debug_message (const char * s, const char * fname)
{
#ifdef INCLUDE_DEBUG_MSGS
  message (L_DEBUG, s, fname);
#endif
}

inline static void
errno_msg(int severity, char const * fname)
{
	message(severity, strerror(errno), fname);
}

#define enomem_msg _("cannot find enough memory")

inline static void
enomem (const char * fname)
{
  message (L_IMPORTANT, enomem_msg, fname);
}

inline static void
fatal_enomem (const char * fname)
{
  message(L_FATAL, enomem_msg, fname);
  fail();
}

#undef enomem_msg

/* Given a string that represents a log level, return the number that
   repsesents that loglevel.  A negative return value means that the
   string is not a valid log level.  */
int
str2loglevel (const char * s);

/* Set current log level to ll.  */
void
set_loglevel (int ll);

/* Set program name to pn. */
void
msg_set_progname (const char * pn);

/* Return true iff loglevels IMPORTANT or FATAL have been used.  */
static inline
bool errors_reported(void)
{
#ifndef MSG_C__
	extern bool errors;
#endif
	return errors;
}

static inline
void record_error(void)
{
#ifndef MSG_C__
	extern bool errors;
#endif
	errors = 1;
}


#endif /* MSG_H__ */
