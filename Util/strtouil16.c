#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
uint_least16_t strtouil16(const char *Str, char **End, int Base)
{ /* strtouil16(const char *, char **, int) */
#if UINT_LEAST16_MAX == ULONG_MAX
  /* In the event that 'uint_least16_t' and 'unsigned long' are the
   * same size just call strtoul() and cast the result. */
  return (uint_least16_t) strtoul(Str, End, Base);

#elif UINT_LEAST16_MAX < LONG_MAX
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
  if (Val > (long) UINT_LEAST16_MAX || Val < -((long) UINT_LEAST16_MAX))
  { /* Overflow. */
    Val = UINT_LEAST16_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (uint_least16_t) Val;
#else
#error "Can not implement strtouil16()."
#endif
} /* strtouil16(const char *, char **, int) */
