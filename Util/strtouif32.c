#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
uint_fast32_t strtouif32(const char *Str, char **End, int Base)
{ /* strtouif32(const char *, char **, int) */
#if UINT_FAST32_MAX == ULONG_MAX
  /* In the event that 'uint_fast32_t' and 'unsigned long' are the
   * same size just call strtoul() and cast the result. */
  return (uint_fast32_t) strtoul(Str, End, Base);

#elif UINT_FAST32_MAX < LONG_MAX
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
  if (Val > (long) UINT_FAST32_MAX || Val < -((long) UINT_FAST32_MAX))
  { /* Overflow. */
    Val = UINT_FAST32_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (uint_fast32_t) Val;
#else
#error "Can not implement strtouif32()."
#endif
} /* strtouif32(const char *, char **, int) */
