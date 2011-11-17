#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
int8_t strtoi8(const char *Str, char **End, int Base)
{ /* strtoi8(const char *, char **, int) */
#if INT8_MAX == LONG_MAX
  /* In the unlikely event that 'int8_t' and 'long' are the same size
   * just call strtol() and cast the result. */
  return (int8_t) strtol(Str, End, Base);

#elif INT8_MAX < LONG_MAX
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
  if (Val > INT8_MAX)
  { /* Overflow. */
    Val = INT8_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (Val < INT8_MIN)
  { /* Underflow. */
    Val = INT8_MIN;
    if (errno == 0)
      errno = ERANGE;
  } /* Underflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (int8_t) Val;
#else
#error "Can not implement strtoi8()."
#endif
} /* strtoi8(const char *, char **, int) */
