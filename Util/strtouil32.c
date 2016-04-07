#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "StrToX.h"

/******************************************************************************
 * Function equivalent to strtoul() but for 'uint_least32_t'.  See
 * strtous.c for the logic behind this code.o
 *****************************************************************************/
uint_least32_t strtouil32(const char *Str, char **End, int Base)
{ /* strtouil32(const char *, char **, int) */
  /* We really have no idea what basic C type uint_least32_t might be
   * so we need to check against both 'unsigned long' and 'unsigned
   * long long'. */
  
#if UINT_LEAST32_MAX == ULONG_MAX
  /* In the event that 'uint_least32_t' and 'unsigned long' are the
   * same size just call strtoul() and cast the result. */
  return (uint_least32_t) strtoul(Str, End, Base);

#elif UINT_LEAST32_MAX == ULLONG_MAX
  /* In the event that 'uint_least32_t' and 'unsigned long long' are
   * the same size just call strtoull() and cast the result. */
  return (uint_least32_t) strtoull(Str, End, Base);

#elif UINT_LEAST32_MAX < LONG_MAX
  int SavedErrNo;
  long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long'. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Clamp the converted value to the range of an 'uint_least32_t'.
   * If 'errno' is not already set, set it to 'ERANGE' */
  if (Val > (long) UINT_LEAST32_MAX || Val < -((long) UINT_LEAST32_MAX))
  { /* Overflow. */
    Val = UINT_LEAST32_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (uint_least32_t) Val;
#else
#error "Can not implement strtouil32()."
#endif
} /* strtouil32(const char *, char **, int) */
