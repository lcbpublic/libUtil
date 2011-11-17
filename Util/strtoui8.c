#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
uint8_t strtoui8(const char *Str, char **End, int Base)
{ /* strtoui8(const char *, char **, int) */
#if UINT8_MAX == ULONG_MAX
  /* In the unlikely event that 'uint8_t' and 'unsigned long' are the
   * same size just call strtoul() and cast the result. */
  return (uint8_t) strtoul(Str, End, Base);

#elif UINT8_MAX < LONG_MAX
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
   * To correctly mirror strtoul() for uint8_t we must return ERANGE
   * for anything greater than 0xff or less than -0xff.  We do this
   * by calling strtol() (signed long), checking the (signed) range
   * of the result, and finally casting to uint8_t.  This is
   * admittedly nasty but I don't see any other good way of doing
   * it. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Check for errors. */
  if (Val > (long) UINT8_MAX || Val < -((long) UINT8_MAX))
  { /* Overflow. */
    Val = UINT8_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (uint8_t) Val;
#else
#error "Can not implement strtoui8()."
#endif
} /* strtoui8(const char *, char **, int) */
