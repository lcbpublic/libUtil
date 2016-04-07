#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "StrToX.h"

/******************************************************************************
 * Function equivalent to strtoul() but for 'uint_fast16_t'.  See
 * strtous.c for the logic behind this code.
 *****************************************************************************/
uint_fast16_t strtouif16(const char *Str, char **End, int Base)
{ /* strtouif16(const char *, char **, int) */
  /* We really have no idea what basic C type uint_fast16_t might be
   * so we need to check against both 'unsigned long' and 'unsigned
   * long long'. */
  
#if UINT_FAST16_MAX == ULONG_MAX
  /* In the event that 'uint_fast16_t' and 'unsigned long' are the
   * same size just call strtoul() and cast the result. */
  return (uint_fast16_t) strtoul(Str, End, Base);

#elif UINT_FAST16_MAX == ULLONG_MAX
  /* In the event that 'uint_fast16_t' and 'unsigned long long' are
   * the same size just call strtoull() and cast the result. */
  return (uint_fast16_t) strtoull(Str, End, Base);

#elif UINT_FAST16_MAX < LONG_MAX
  int SavedErrNo;
  long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long'. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Clamp the converted value to the range of an 'uint_fast16_t'.  If
   * 'errno' is not already set, set it to 'ERANGE' */
  if (Val > (long) UINT_FAST16_MAX || Val < -((long) UINT_FAST16_MAX))
  { /* Overflow. */
    Val = UINT_FAST16_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (uint_fast16_t) Val;
#else
#error "Can not implement strtouif16()."
#endif
} /* strtouif16(const char *, char **, int) */
