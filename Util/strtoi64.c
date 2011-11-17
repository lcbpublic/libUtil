#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
int64_t strtoi64(const char *Str, char **End, int Base)
{ /* strtoi64(const char *, char **, int) */
#if INT64_MAX == LONG_MAX
  /* In the event that 'int64_t' and 'long' are the same size
   * just call strtol() and cast the result. */
  return (int64_t) strtol(Str, End, Base);

#elif INT64_MAX == LLONG_MAX
  /* In the event that 'int64_t' and 'long long' are the same size
   * just call strtoll() and cast the result. */
  return (int64_t) strtoll(Str, End, Base);

#elif INT64_MAX < LONG_MAX
  int SavedErrNo;
  long long Val;

  /* 
   * If we are successful we must not modify errno.  However,
   * to reliably detect errors in strtoul() we must set errno
   * to zero.  So, ...
   */
  SavedErrNo = errno;

  /* Convert to long. */
  errno = 0;
  Val = strtoll(Str, End, Base);

  /* Check for errors. */
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

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (int64_t) Val;
#else
#error "Can not implement strtoi64()."
#endif
} /* strtoi64(const char *, char **, int) */
