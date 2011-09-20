#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
unsigned short strtous(const char *Str, char **End, int Base)
{ /* strtous(char *, char **, int) */
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
    if (Val > USHRT_MAX)
      Val = USHRT_MAX;
    return (unsigned short) Val;
  } /* Error. */

  /* Check range. */
  if (Val > USHRT_MAX && (long) Val < SHRT_MIN)
  { /* Range error. */
    Val = USHRT_MAX;
    errno = ERANGE;
    return (unsigned short) Val;
  } /* Range error. */

  /* Return result(s). */
  errno = SavedErrNo;
  if (End != NULL)
    *End = (char *) Src;
  return (unsigned short) Val;
} /* strtous(char *, char **, int) */
