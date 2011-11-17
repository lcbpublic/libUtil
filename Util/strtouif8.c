#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
uint_fast8_t strtouif8(const char *Str, char **End, int Base)
{ /* strtouif8(const char *, char **, int) */
#if UINT_FAST8_MAX == ULONG_MAX
  /* In the event that 'uint_fast8_t' and 'unsigned long' are the
   * same size just call strtoul() and cast the result. */
  return (uint_fast8_t) strtoul(Str, End, Base);

#elif UINT_FAST8_MAX < LONG_MAX
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
  if (Val > (long) UINT_FAST8_MAX || Val < -((long) UINT_FAST8_MAX))
  { /* Overflow. */
    Val = UINT_FAST8_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  /* Return result. */
  if (errno == 0)
    errno = SavedErrNo;
  return (uint_fast8_t) Val;
#else
#error "Can not implement strtouif8()."
#endif
} /* strtouif8(const char *, char **, int) */
