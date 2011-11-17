#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
int strtoi(const char *Str, char **End, int Base)
{ /* strtoi(char *, char **, int) */
#if INT_MAX == LONG_MAX
  /* In the event that 'int' and 'long' are the same size just call
   * strtol() and cast the result. */
  return (int) strtoul(Str, End, Base);

#elif INTT_MAX < LONG_MAX
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
  if (Val > INT_MAX)
  { /* Overflow. */
    Val = INT_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (Val < INT_MIN)
  { /* Underflow. */
    Val = INT_MIN;
    if (errno == 0)
      errno = ERANGE;
  } /* Underflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (int) Val;
#else
#error "Can not implement strtoui()."
#endif
} /* strtoi(char *, char **, int) */
