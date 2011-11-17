#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
uint16_t strtoui16(const char *Str, char **End, int Base)
{ /* strtoui16(const char *, char **, int) */
#if UINT16_MAX == ULONG_MAX
  /* In the event that 'uint16_t' and 'unsigned long' are the same
   * size just call strtoul() and cast the result. */
  return (uint16_t) strtoul(Str, End, Base);

#elif UINT16_MAX < LONG_MAX
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
   * To correctly mirror strtoul() for uint16_t we must return
   * ERANGE for anything greater than 0xffff or less than -0xffff.
   * We do this by calling strtol() (signed long), checking the
   * (signed) range of the result, and finally casting to uint16_t.
   * This is admittedly nasty but I don't see any other good way of
   * doing it. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Check for errors. */
  if (Val > (long) UINT16_MAX || Val < -((long) UINT16_MAX))
  { /* Overflow. */
    Val = UINT16_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (uint16_t) Val;
#else
#error "Can not implement strtoui16()."
#endif
} /* strtoui16(const char *, char **, int) */
