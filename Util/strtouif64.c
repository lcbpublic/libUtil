#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
uint_fast64_t strtouif64(const char *Str, char **End, int Base)
{ /* strtouif64(const char *, char **, int) */
#if UINT_FAST64_MAX == ULONG_MAX
  /* In the event that 'uint_fast64_t' and 'unsigned long' are the
   * same size just call strtoul() and cast the result. */
  return (uint_fast64_t) strtoul(Str, End, Base);

#elif UINT_FAST64_MAX == ULLONG_MAX
  /* In the event that 'uint_fast64_t' and 'unsigned long long' are
   * the same size just call strtoull() and cast the result. */
  return (uint_fast64_t) strtoull(Str, End, Base);

#elif UINT_FAST64_MAX < LONG_MAX
  int SavedErrNo;
  long long Val;

  /* 
   * If we are successful we must not modify errno.  However,
   * to reliably detect errors in strtoul() we must set errno
   * to zero.  So, ...
   */
  SavedErrNo = errno;

  /* Convert to long long. */
  errno = 0;
  Val = strtoll(Str, End, Base);

  /* Check for errors. */
  if (Val > (long long) UINT_FAST64_MAX ||
      Val < -((long long) UINT_FAST64_MAX))
  { /* Overflow. */
    Val = UINT_FAST64_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (uint_fast64_t) Val;
#else
#error "Can not implement strtouif64()."
#endif
} /* strtouif64(const char *, char **, int) */
