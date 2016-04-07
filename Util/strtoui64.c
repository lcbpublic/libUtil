#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "StrToX.h"

/******************************************************************************
 * Function equivalent to strtoull() but for 'uint64_t'.  See
 * strtous.c for the logic behind this code.
 *****************************************************************************/
uint64_t strtoui64(const char *Str, char **End, int Base)
{ /* strtoui64(const char *, char **, int) */
  /* We really have no idea what basic C type uint64_t might be so we
   * need to check against both 'unsigned long' and 'unsigned long
   * long'. */
  
#if UINT64_MAX == ULONG_MAX
  /* In the event that 'uint64_t' and 'unsigned long' are the same
   * size just call strtoul() and cast the result. */
  return (uint64_t) strtoul(Str, End, Base);

#elif UINT64_MAX == ULLONG_MAX
  /* In the event that 'uint64_t' and 'unsigned long long' are the
   * same size just call strtoull() and cast the result. */
  return (uint64_t) strtoull(Str, End, Base);

#elif UINT64_MAX < LLONG_MAX
  int SavedErrNo;
  long long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long long'. */
  errno = 0;
  Val = strtoll(Str, End, Base);

  /* Clamp the converted value to the range of an 'uint64_t'.  If
   * 'errno' is not already set, set it to 'ERANGE' */
  if (Val > (long long) UINT64_MAX || Val < -((long long) UINT64_MAX))
  { /* Overflow. */
    Val = UINT64_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (uint64_t) Val;
#else
#error "Can not implement strtoui64()."
#endif
} /* strtoui64(const char *, char **, int) */
