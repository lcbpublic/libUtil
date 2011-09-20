#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
short strtos(const char *Str, char **End, int Base)
{ /* strtos(char *, char **, int) */
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
    if (Val > SHRT_MAX)
      Val = SHRT_MAX;
    else if (Val < SHRT_MIN)
      Val = SHRT_MIN;
    return (short) Val;
  } /* Error. */

  /* Check range. */
  if (Val > SHRT_MAX)
  { /* Range error. */
    Val = SHRT_MAX;
    errno = ERANGE;
    return (short) Val;
  } /* Range error. */

  if (Val < SHRT_MIN)
  { /* Range error. */
    Val = SHRT_MIN;
    errno = ERANGE;
    return (short) Val;
  } /* Range error. */

  /* Return result(s). */
  errno = SavedErrNo;
  if (End != NULL)
    *End = (char *) Src;
  return (short) Val;
} /* strtos(char *, char **, int) */
