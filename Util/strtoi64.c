#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Function equivalent to strtol() but for 'int64_t'.  See strtos.c
 * for the logic behind this code.
 *****************************************************************************/
int64_t strtoi64(const char *Str, char **End, int Base)
{ /* strtoi64(const char *, char **, int) */
#if INT64_MAX == LONG_MAX && INT64_MIN == LONG_MIN
  /* In the event that 'int64_t' and 'long' are the same size
   * just call strtoll() and cast the result. */
  return (int64_t) strtol(Str, End, Base);

#elif INT64_MAX == LLONG_MAX && INT64_MIN == LLONG_MIN
  /* In the event that 'int64_t' and 'long long' are the same size
   * just call strtoll() and cast the result. */
  return (int64_t) strtoll(Str, End, Base);

#elif INT64_MAX < LLONG_MAX && INT64_MIN > LLONG_MIN
  int SavedErrNo;
  long long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to long long. */
  errno = 0;
  Val = strtoll(Str, End, Base);

  /* Clamp the converted value to the range of an 'int64_t'.  If
   * 'errno' is not already set, set it to 'ERANGE' */
  if (Val > INT64_MAX)
  { /* Overflow. */
    Val = INT64_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (Val < INT64_MIN)
  { /* Underflow. */
    Val = INT64_MIN;
    if (errno == 0)
      errno = ERANGE;
  } /* Underflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (int64_t) Val;
#else
#error "Can not implement strtoi64()."
#endif
} /* strtoi64(const char *, char **, int) */
