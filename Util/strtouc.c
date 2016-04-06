#include <errno.h>
#include <limits.h>

#include "StrTo.h"

/******************************************************************************
 * Function equivalent to strtol() but for 'unsigned char'.  See
 * strtous.c for the logic behind this code.o
 *****************************************************************************/
unsigned char strtouc(const char *Str, char **End, int Base)
{ /* strtouc(const char *, char **, int) */
#if UCHAR_MAX == ULONG_MAX
  /* In the event that 'unsigned char' and 'unsigned long' are the same
   * size just call strtoul() and cast the result. */
  return (unsigned char) strtoul(Str, End, Base);

#elif UCHAR_MAX < LONG_MAX
  int SavedErrNo;
  long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long'. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Clamp the converted value to the range of an 'unsigned char'.  If
   * 'errno' is not already set, set it to 'ERANGE' */
  if (Val > (long) UCHAR_MAX || Val < -((long) UCHAR_MAX))
  { /* Overflow. */
    Val = UCHAR_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */
  
  /* Return the result. */
  return (unsigned char) Val;
#else
#error "Can not implement strtouc()."
#endif
} /* strtouc(const char *, char **, char) */
