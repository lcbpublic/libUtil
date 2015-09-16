#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Function equivalent to strtol() but for 'int_least8_t'.  See
 * strtos.c for the logic behind this code.
 *****************************************************************************/
int_least8_t strtoil8(const char *Str, char **End, int Base)
{ /* strtoil8(const char *, char **, int) */
  /* We really have no idea what basic C type int_least8_t might be so
   * we need to check against both 'long' and 'long long'. */
  
#if INT_LEAST8_MAX == LONG_MAX && INT_LEAST8_MIN == LONG_MIN
  /* In the event that 'int_least8_t' and 'long' are the same size
   * just call strtol() and cast the result. */
  return (int_least8_t) strtol(Str, End, Base);

#elif INT_LEAST8_MAX == LLONG_MAX && INT_LEAST8_MIN == LLONG_MIN
  /* In the event that 'int_least8_t' and 'long long' are the same
   * size just call strtoll() and cast the result. */
  return (int_least8_t) strtoll(Str, End, Base);

#elif INT_LEAST8_MAX < LONG_MAX && INT_LEAST8_MIN > LONG_MIN
  int SavedErrNo;
  long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long'. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Clamp the converted value to the range of an 'int_least8_t'.  If
   * 'errno' is not already set, set it to 'ERANGE' */
  if (Val > INT_LEAST8_MAX)
  { /* Overflow. */
    Val = INT_LEAST8_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (Val < INT_LEAST8_MIN)
  { /* Underflow. */
    Val = INT_LEAST8_MIN;
    if (errno == 0)
      errno = ERANGE;
  } /* Underflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (int_least8_t) Val;
#else
#error "Can not implement strtoil8()."
#endif
} /* strtoil8(const char *, char **, int) */
