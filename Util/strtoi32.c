#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "StrToX.h"

/******************************************************************************
 * Function equivalent to strtol() but for 'int32_t'.  See strtos.c
 * for the logic behind this code.
 *****************************************************************************/
int32_t strtoi32(const char *Str, char **End, int Base)
{ /* strtoi32(const char *, char **, int) */
  /* We really have no idea what basic C type int32_t might be so
   * we need to check against both 'long' and 'long long'. */
  
#if INT32_MAX == LONG_MAX && INT32_MIN == LONG_MIN
  /* In the event that 'int32_t' and 'long' are the same size just call
   * strtol() and cast the result. */
  return (int32_t) strtol(Str, End, Base);

#elif INT32_MAX == LLONG_MAX && INT32_MIN == LLONG_MIN
  /* In the event that 'int32_t' and 'long long' are the same size
   * just call strtoll() and cast the result. */
  return (int32_t) strtoll(Str, End, Base);

#elif INT32_MAX < LONG_MAX && INT32_MIN > LONG_MIN
  int SavedErrNo;
  long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long'. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Clamp the converted value to the range of an 'int32_t'.  If
   * 'errno' is not already set, set it to 'ERANGE' */
  if (Val > INT32_MAX)
  { /* Overflow. */
    Val = INT32_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (Val < INT32_MIN)
  { /* Underflow. */
    Val = INT32_MIN;
    if (errno == 0)
      errno = ERANGE;
  } /* Underflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (int32_t) Val;
#else
#error "Can not implement strtoi32()."
#endif
} /* strtoi32(const char *, char **, int) */
