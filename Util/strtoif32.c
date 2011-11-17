#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
int_fast32_t strtoif32(const char *Str, char **End, int Base)
{ /* strtoif32(const char *, char **, int) */
#if INT_FAST32_MAX == LONG_MAX
  /* In the event that 'int_fast32_t' and 'long' are the same size
   * just call strtol() and cast the result. */
  return (int_fast32_t) strtol(Str, End, Base);

#elif INT_FAST32_MAX < LONG_MAX
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

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (int_fast32_t) Val;
#else
#error "Can not implement strtoif32()."
#endif
} /* strtoif32(const char *, char **, int) */
