#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
int_fast16_t strtoif16(const char *Str, char **End, int Base)
{ /* strtoif16(const char *, char **, int) */
#if INT_FAST16_MAX == LONG_MAX
  /* In the event that 'int_fast16_t' and 'long' are the same size
   * just call strtol() and cast the result. */
  return (int_fast16_t) strtol(Str, End, Base);

#elif INT_FAST16_MAX < LONG_MAX
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
  if (Val > INT_FAST16_MAX)
  { /* Overflow. */
    Val = INT_FAST16_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (Val < INT_FAST16_MIN)
  { /* Underflow. */
    Val = INT_FAST16_MIN;
    if (errno == 0)
      errno = ERANGE;
  } /* Underflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (int_fast16_t) Val;
#else
#error "Can not implement strtoif16()."
#endif
} /* strtoif16(const char *, char **, int) */
