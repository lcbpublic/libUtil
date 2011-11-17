#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
unsigned int strtoui(const char *Str, char **End, int Base)
{ /* strtoui(char *, char **, int) */
#if UINT_MAX == ULONG_MAX
  /* In the event that 'unsigned int' and 'unsigned long' are the same
   * size just call strtoul() and cast the result. */
  return (unsigned int) strtoul(Str, End, Base);

#elif UINT_MAX < LONG_MAX
  int SavedErrNo;
  long Val;

  /* 
   * If we are successful we must not modify errno.  However,
   * to reliably detect errors in strtoul() we must set errno
   * to zero.  So, ...
   */
  SavedErrNo = errno;

  /* Convert to long.  strtoul() silently converts negatives to
   * positives so '-1' is indistinguishable from 0xffffffffffffffff.
   * To correctly mirror strtoul() for unsigned int we must return
   * ERANGE for anything greater than 0xffffffff or less than
   * -0xffffffff.  We do this by calling strtol() (signed long),
   * checking the (signed) range of the result, and finally casting
   * to unsigned int.  This is admittedly nasty but I don't see any
   * other good way of doing it. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Check for errors. */
  if (Val > (long) UINT_MAX || Val < -((long) UINT_MAX))
  { /* Overflow. */
    Val = UINT_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (unsigned int) Val;
#else
#error "Can not implement strtoui()."
#endif
} /* strtoui(char *, char **, int) */
