#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
int8_t strtob(const char *Str, char **End, int Base)
{ /* strtob(char *, char **, int) */
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
    if (Val > INT8_MAX)
      Val = INT8_MAX;
    else if (Val < INT8_MIN)
      Val = INT8_MIN;
    return (int8_t) Val;
  } /* Error. */

  /* Check range. */
  if (Val > INT8_MAX)
  { /* Range error. */
    Val = INT8_MAX;
    errno = ERANGE;
    return (int8_t) Val;
  } /* Range error. */

  if (Val < INT8_MIN)
  { /* Range error. */
    Val = INT8_MIN;
    errno = ERANGE;
    return (int8_t) Val;
  } /* Range error. */

  /* Return result(s). */
  errno = SavedErrNo;
  if (End != NULL)
    *End = (char *) Src;
  return (int8_t) Val;
} /* strtob(char *, char **, int) */
