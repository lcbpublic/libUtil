#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "StrTo.h"

/******************************************************************************
 * Function equivalent to strtoul() but for 'uint16_t'.  See strtous.c
 * for the logic behind this code.
 *****************************************************************************/
uint16_t strtoui16(const char *Str, char **End, int Base)
{ /* strtoui16(const char *, char **, int) */
  /* We really have no idea what basic C type uint16_t might be so we
   * need to check against both 'unsigned long' and 'unsigned long
   * long'. */
  
#if UINT16_MAX == ULONG_MAX
  /* In the event that 'uint16_t' and 'unsigned long' are the
   * same size just call strtoul() and cast the result. */
  return (uint16_t) strtoul(Str, End, Base);

#elif UINT16_MAX == ULLONG_MAX
  /* In the event that 'uint16_t' and 'unsigned long long' are the
   * same size just call strtoull() and cast the result. */
  return (uint16_t) strtoull(Str, End, Base);

#elif UINT16_MAX < LONG_MAX
  int SavedErrNo;
  long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long'. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Clamp the converted value to the range of an 'uint16_t'.  If
   * 'errno' is not already set, set it to 'ERANGE' */
  if (Val > (long) UINT16_MAX || Val < -((long) UINT16_MAX))
  { /* Overflow. */
    Val = UINT16_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (uint16_t) Val;
#else
#error "Can not implement strtoui16()."
#endif
} /* strtoui16(const char *, char **, int) */
