#include <errno.h>
#include <limits.h>

#include "StrTo.h"

/******************************************************************************
 * Function equivalent to strtol() but for 'signed char'.  See
 * strtos.c for the logic behind this code.
 *****************************************************************************/
signed char strtosc(const char *Str, char **End, int Base)
{ /* strtosc(const char *, char **, int) */
#if SCHAR_MAX == LONG_MAX && SCHAR_MIN > LONG_MIN
  /* In the event that 'signed char' and 'long' are the same size just call
   * strtol() and cast the result. */
  return (signed char) strtoul(Str, End, Base);

#elif SCHAR_MAX < LONG_MAX && SCHAR_MIN > LONG_MIN 
  int SavedErrNo;
  long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long'. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Clamp the converted value to the range of a 'signed char'.  If
   * 'errno' is not already set, set it to 'ERANGE' */
  if (Val > SCHAR_MAX)
  { /* Overflow. */
    Val = SCHAR_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (Val < SCHAR_MIN)
  { /* Underflow. */
    Val = SCHAR_MIN;
    if (errno == 0)
      errno = ERANGE;
  } /* Underflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (signed char) Val;
#else
#error "Can not implement strtosc()."
#endif
} /* strtosc(const char *, char **, int) */
