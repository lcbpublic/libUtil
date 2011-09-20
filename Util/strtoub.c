#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
uint8_t strtoub(const char *Str, char **End, int Base)
{ /* strtoub(char *, char **, int) */
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
    if (Val > UINT8_MAX)
      Val = UINT8_MAX;
    return (uint8_t) Val;
  } /* Error. */

  /* Check range. */
  if (Val > UINT8_MAX && (long) Val < INT8_MIN)
  { /* Range error. */
    Val = UINT8_MAX;
    errno = ERANGE;
    return (uint8_t) Val;
  } /* Range error. */

  /* Return result(s). */
  errno = SavedErrNo;
  if (End != NULL)
    *End = (char *) Src;
  return (uint8_t) Val;
} /* strtoub(char *, char **, int) */
