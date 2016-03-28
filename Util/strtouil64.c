#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "StrTo.h"

/******************************************************************************
 * Function equivalent to strtoull() but for 'uint_least64_t'.  See
 * strtous.c for the logic behind this code.o
 *****************************************************************************/
uint_least64_t strtouil64(const char *Str, char **End, int Base)
{ /* strtouil64(const char *, char **, int) */
  /* We really have no idea what basic C type uint_least64_t might be
   * so we need to check against both 'unsigned long' and 'unsigned
   * long long'. */
  
#if UINT_LEAST64_MAX == ULONG_MAX
  /* In the event that 'uint_least64_t' and 'unsigned long' are the
   * same size just call strtoul() and cast the result. */
  return (uint_least64_t) strtoul(Str, End, Base);

#elif UINT_LEAST64_MAX == ULLONG_MAX
  /* In the event that 'uint_least64_t' and 'unsigned long long' are
   * the same size just call strtoull() and cast the result. */
  return (uint_least64_t) strtoull(Str, End, Base);

#elif UINT_LEAST64_MAX < LLONG_MAX
  int SavedErrNo;
  long long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long long'. */
  errno = 0;
  Val = strtoll(Str, End, Base);

  /* Clamp the converted value to the range of an 'uint_least64_t'.
   * If 'errno' is not already set, set it to 'ERANGE' */
  if (Val > (long long) UINT_LEAST64_MAX ||
      Val < -((long long) UINT_LEAST64_MAX))
  { /* Overflow. */
    Val = UINT_LEAST64_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (uint_least64_t) Val;
#else
#error "Can not implement strtouil64()."
#endif
} /* strtouil64(const char *, char **, int) */
