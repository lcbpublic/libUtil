#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "StrToX.h"

/******************************************************************************
 * Function equivalent to strtol() but for 'int_fast32_t'.  See
 * strtos.c for the logic behind this code.
 *****************************************************************************/
int_fast32_t strtoif32(const char *Str, char **End, int Base)
{ /* strtoif32(const char *, char **, int) */
  /* We really have no idea what basic C type int_fast32_t might be so
   * we need to check against both 'long' and 'long long'. */
  
#if INT_FAST32_MAX == LONG_MAX && INT_FAST32_MIN == LONG_MIN
  /* In the event that 'int_fast32_t' and 'long' are the same size
   * just call strtol() and cast the result. */
  return (int_fast32_t) strtol(Str, End, Base);

#elif INT_FAST32_MAX == LLONG_MAX && INT_FAST32_MIN == LLONG_MIN
  /* In the event that 'int_fast32_t' and 'long long' are the same
   * size just call strtoll() and cast the result. */
  return (int_fast32_t) strtoll(Str, End, Base);

#elif INT_FAST32_MAX < LONG_MAX && INT_FAST32_MIN > LONG_MIN
  int SavedErrNo;
  long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long'. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Clamp the converted value to the range of an 'int_fast32_t'.  If
   * 'errno' is not already set, set it to 'ERANGE' */
  if (Val > INT_FAST32_MAX)
  { /* Overflow. */
    Val = INT_FAST32_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (Val < INT_FAST32_MIN)
  { /* Underflow. */
    Val = INT_FAST32_MIN;
    if (errno == 0)
      errno = ERANGE;
  } /* Underflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (int_fast32_t) Val;
#else
#error "Can not implement strtoif32()."
#endif
} /* strtoif32(const char *, char **, int) */
