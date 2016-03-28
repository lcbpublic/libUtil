#include <errno.h>
#include <limits.h>

#include "StrTo.h"

/******************************************************************************
 * Function equivalent to strtol() but for 'unsigned int'.  See
 * strtous.c for the logic behind this code.o
 *****************************************************************************/
unsigned int strtoui(const char *Str, char **End, int Base)
{ /* strtoui(const char *, char **, int) */
#if UINT_MAX == ULONG_MAX
  /* In the event that 'unsigned int' and 'unsigned long' are the same
   * size just call strtoul() and cast the result. */
  return (unsigned int) strtoul(Str, End, Base);

#elif UINT_MAX < LONG_MAX
  int SavedErrNo;
  long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long'. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Clamp the converted value to the range of an 'unsigned int'.  If
   * 'errno' is not already set, set it to 'ERANGE' */
  if (Val > (long) UINT_MAX || Val < -((long) UINT_MAX))
  { /* Overflow. */
    Val = UINT_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */
  
  /* Return the result. */
  return (unsigned int) Val;
#else
#error "Can not implement strtoui()."
#endif
} /* strtoui(const char *, char **, int) */
