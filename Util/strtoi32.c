#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
int32_t strtoi32(const char *Str, char **End, int Base)
{ /* strtoi32(const char *, char **, int) */
#if INT32_MAX == LONG_MAX
  /* In the event that 'int32_t' and 'long' are the same size just
   * call strtol() and cast the result. */
  return (int32_t) strtol(Str, End, Base);

#elif INT32_MAX < LONG_MAX
  int SavedErrNo;
  long Val;

  /* 
   * If we are successful we must not modify errno.  However,
   * to reliably detect errors in strtoul() we must set errno
   * to zero.  So, ...
   */
  SavedErrNo = errno;

  /* Convert to long. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Check for errors. */
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

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (int32_t) Val;
#else
#error "Can not implement strtoi32()."
#endif
} /* strtoi32(const char *, char **, int) */
