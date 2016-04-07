#include <errno.h>
#include <limits.h>

#include "StrToX.h"

/******************************************************************************
 * Function equivalent to strtol() but for 'int'.  See strtos.c for the
 * logic behind this code.
 *****************************************************************************/
int strtoi(const char *Str, char **End, int Base)
{ /* strtoi(const char *, char **, int) */
#if INT_MAX == LONG_MAX && INT_MIN > LONG_MIN
  /* In the event that 'int' and 'long' are the same size just call
   * strtol() and cast the result. */
  return (int) strtoul(Str, End, Base);

#elif INT_MAX < LONG_MAX && INT_MIN > LONG_MIN 
  int SavedErrNo;
  long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long'. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Clamp the converted value to the range of a 'int'.  If 'errno' is
   * not already set, set it to 'ERANGE' */
  if (Val > INT_MAX)
  { /* Overflow. */
    Val = INT_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (Val < INT_MIN)
  { /* Underflow. */
    Val = INT_MIN;
    if (errno == 0)
      errno = ERANGE;
  } /* Underflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (int) Val;
#else
#error "Can not implement strtoi()."
#endif
} /* strtoi(const char *, char **, int) */
