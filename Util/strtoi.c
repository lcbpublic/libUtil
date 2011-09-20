#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
int strtoi(const char *Str, char **End, int Base)
{ /* strtoi(char *, char **, int) */
  const char *Src;
  char *EndPtr;
  int SavedErrNo;
  long Val;

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
  Val = strtol(Src, &EndPtr, Base);
  Src = EndPtr;
  if (errno != 0)
  { /* Error. */
    if (Val > INT_MAX)
      Val = INT_MAX;
    else if (Val < INT_MIN)
      Val = INT_MIN;
    return (int) Val;
  } /* Error. */

  /* Check range. */
  if (Val > INT_MAX)
  { /* Range error. */
    Val = INT_MAX;
    errno = ERANGE;
    return (int) Val;
  } /* Range error. */

  if (Val < INT_MIN)
  { /* Range error. */
    Val = INT_MIN;
    errno = ERANGE;
    return (int) Val;
  } /* Range error. */

  /* Return result(s). */
  errno = SavedErrNo;
  if (End != NULL)
    *End = (char *) Src;
  return (int) Val;
} /* strtoi(char *, char **, int) */
