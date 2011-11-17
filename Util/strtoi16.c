#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
int16_t strtoi16(const char *Str, char **End, int Base)
{ /* strtoi16(const char *, char **, int) */
#if INT16_MAX == LONG_MAX
  /* If 'int16_t' and 'long' are the same size just call strtol() and
   * cast the result. */
  return (int16_t) strtol(Str, End, Base);

#elif INT16_MAX < LONG_MAX
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

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (int16_t) Val;
#else
#error "Can not implement strtoi16()."
#endif
} /* strtoi16(const char *, char **, int) */
