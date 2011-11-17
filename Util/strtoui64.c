#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
uint64_t strtoui64(const char *Str, char **End, int Base)
{ /* strtoui64(const char *, char **, int) */
#if UINT64_MAX == ULONG_MAX
  /* In the event that 'uint64_t' and 'unsigned long' are the same
   * size just call strtoul() and cast the result. */
  return (uint64_t) strtoul(Str, End, Base);

#elif UINT64_MAX == ULLONG_MAX
  /* In the event that 'uint64_t' and 'unsigned long long' are the
   * same size just call strtoull() and cast the result. */
  return (uint64_t) strtoull(Str, End, Base);

#elif UINT64_MAX < LONG_MAX
  int SavedErrNo;
  long long Val;

  /* 
   * If we are successful we must not modify errno.  However,
   * to reliably detect errors in strtoul() we must set errno
   * to zero.  So, ...
   */
  SavedErrNo = errno;

  /* Convert to long long.  strtoull() silently converts negatives
   * to positives so '-1' is indistinguishable from
   * 0xffffffffffffffff.  To correctly mirror strtoull() for uint64_t
   * we must return ERANGE for anything greater than 0xff or less
   * than -0xff.  We do this by calling strtoll() (signed long
   * long), checking the (signed) range of the result, and finally
   * casting to uint64_t.  This is admittedly nasty but I don't see
   * any other good way of doing it. */
  errno = 0;
  Val = strtoll(Str, End, Base);

  /* Check for errors. */
  if (Val > (long long) UINT64_MAX || Val < -((long long) UINT64_MAX))
  { /* Overflow. */
    Val = UINT64_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (uint64_t) Val;
#else
#error "Can not implement strtoui64()."
#endif
} /* strtoui64(const char *, char **, int) */
