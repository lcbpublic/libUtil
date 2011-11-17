#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
unsigned short strtous(const char *Str, char **End, int Base)
{ /* strtous(char *, char **, int) */
#if USHRT_MAX == ULONG_MAX
  /* In the event that 'unsigned short' and 'unsigned long' are the
   * same size just call strtoul() and cast the result. */
  return (unsigned short) strtoul(Str, End, Base);

#elif USHRT_MAX < LONG_MAX
  /* If a 'long' can represent an 'unsigned short' then call strtol()
   * and check the result.  See comments below for more details. */
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
   * To correctly mirror strtoul() for unsigned short we must return
   * ERANGE for anything greater than 0xffff or less than -0xffff.
   * We do this by calling strtol() (signed long), checking the
   * (signed) range of the result, and finally casting to unsigned
   * short.  This is admittedly nasty but I don't see any other good
   * way of doing it. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Check for errors. */
  if (Val > (long) USHRT_MAX || Val < -((long) USHRT_MAX))
  { /* Overflow. */
    Val = USHRT_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (unsigned short) Val;
#else
#error "Can not implement strtous()."
#endif
} /* strtous(char *, char **, int) */
