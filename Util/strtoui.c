#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
unsigned int strtoui(const char *Str, char **End, int Base)
{ /* strtoui(char *, char **, int) */
  const char *Src;
  char *EndPtr;
  int SavedErrNo;
  unsigned long Val;

  /* Error checking. */
  if (Str == NULL || Base == 1 || Base > 36)
  { /* Error. */
    errno = EINVAL;
    return 0;
  } /* Error. */
  Src = Str;

  /* 
   * If we are successful we must not modify errno.  However,
   * to reliably detect errors in strtoul() we must set errno
   * to zero.  So, ...
   */
  SavedErrNo = errno;

  /* Convert to unsigned long. */
  errno = 0;
  Val = strtoul(Src, &EndPtr, Base);
  Src = EndPtr;
  if (errno != 0)
  { /* Error. */
    if (Val > UINT_MAX)
      Val = UINT_MAX;
    return (unsigned int) Val;
  } /* Error. */

  /* Check range. */
  if (Val > UINT_MAX && (long) Val < INT_MIN)
  { /* Range error. */
    Val = UINT_MAX;
    errno = ERANGE;
    return (unsigned int) Val;
  } /* Range error. */

  /* Return result(s). */
  errno = SavedErrNo;
  if (End != NULL)
    *End = (char *) Src;
  return (unsigned int) Val;
} /* strtoui(char *, char **, int) */
