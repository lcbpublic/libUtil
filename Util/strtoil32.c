#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
int_least32_t strtoil32(const char *Str, char **End, int Base)
{ /* strtoil32(const char *, char **, int) */
#if INT_LEAST32_MAX == LONG_MAX
  /* In the event that 'int_least32_t' and 'long' are the same size
   * just call strtol() and cast the result. */
  return (int_least32_t) strtol(Str, End, Base);

#elif INT_LEAST32_MAX < LONG_MAX
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
  if (Val > INT_LEAST32_MAX)
  { /* Overflow. */
    Val = INT_LEAST32_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (Val < INT_LEAST32_MIN)
  { /* Underflow. */
    Val = INT_LEAST32_MIN;
    if (errno == 0)
      errno = ERANGE;
  } /* Underflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (int_least32_t) Val;
#else
#error "Can not implement strtoil32()."
#endif
} /* strtoil32(const char *, char **, int) */
