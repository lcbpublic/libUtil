#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "StrToX.h"

/******************************************************************************
 * Function equivalent to strtol() but for 'int16_t'.  See strtos.c
 * for the logic behind this code.
 *****************************************************************************/
int16_t strtoi16(const char *Str, char **End, int Base)
{ /* strtoi16(const char *, char **, int) */
  /* We really have no idea what basic C type int16_t might be so
   * we need to check against both 'long' and 'long long'. */
  
#if INT16_MAX == LONG_MAX && INT16_MIN == LONG_MIN
  /* In the event that 'int16_t' and 'long' are the same size just call
   * strtol() and cast the result. */
  return (int16_t) strtol(Str, End, Base);

#elif INT16_MAX == LLONG_MAX && INT16_MIN == LLONG_MIN
  /* In the event that 'int16_t' and 'long long' are the same size
   * just call strtoll() and cast the result. */
  return (int16_t) strtoll(Str, End, Base);

#elif INT16_MAX < LONG_MAX && INT16_MIN > LONG_MIN
  int SavedErrNo;
  long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long'. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Clamp the converted value to the range of an 'int16_t'.  If
   * 'errno' is not already set, set it to 'ERANGE' */
  if (Val > INT16_MAX)
  { /* Overflow. */
    Val = INT16_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (Val < INT16_MIN)
  { /* Underflow. */
    Val = INT16_MIN;
    if (errno == 0)
      errno = ERANGE;
  } /* Underflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (int16_t) Val;
#else
#error "Can not implement strtoi16()."
#endif
} /* strtoi16(const char *, char **, int) */
