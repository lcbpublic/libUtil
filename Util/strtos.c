#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
short strtos(const char *Str, char **End, int Base)
{ /* strtos(char *, char **, int) */
#if SHRT_MAX == LONG_MAX
  /* In the event that 'short' and 'long' are the same size just call
   * strtol() and cast the result. */
  return (short) strtoul(Str, End, Base);

#elif SHRT_MAX < LONG_MAX
  int SavedErrNo;
  long Val;

  /* 
   * If we are successful we must not modify errno.  However,
   * to reliably detect errors in strtoul() we must set errno
   * to zero.  So, ...
   */
  SavedErrNo = errno;

  /* Convert to unsigned long. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Check for errors. */
  if (Val > SHRT_MAX)
  { /* Overflow. */
    Val = SHRT_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (Val < SHRT_MIN)
  { /* Underflow. */
    Val = SHRT_MIN;
    if (errno == 0)
      errno = ERANGE;
  } /* Underflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (short) Val;
#else
#error "Can not implement strtos()."
#endif
} /* strtos(char *, char **, int) */
