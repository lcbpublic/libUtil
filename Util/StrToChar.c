#include <errno.h>
#include <limits.h>

#include "StrToX.h"

/******************************************************************************
 * Equivalent to 'strtoc()' but with simplified error reporting.
 * Returns the 'errno' value rather than setting the global 'errno'.
 * The global 'errno' is always left unchanged.  It is an error not to
 * use all of 'Str', or for (*Str == '\0').  Returns ERANGE on
 * overflow/underflow and EINVAL for a malformed 'Str'.  If the
 * function returns non-zero then '*Val' is unchanged.  In all cases
 * neither 'Str' nor 'Val' may be NULL.
 *****************************************************************************/
int StrToChar(const char *Str, char *Val)
{ /* StrToChar(char *, char *) */
  char NewVal;
  char *End = NULL;
  int SavedErrNo, ErrNo;

  /* NULL checking. */
  if (Str == NULL || Val == NULL)
  { /* Error. */
    return EFAULT;
  } /* Error. */

  /* Check that 'Str' is not empty. */
  if (*Str == '\0')
  { /* Error. */
    return EINVAL;
  } /* Error. */

  /* strtoc() returns error status in 'errno' but we must leave it
   * unchanged. */
  SavedErrNo = errno;

  /* Do the conversion. */
  errno = 0;
  NewVal = strtochar(Str, &End);
  ErrNo = errno;

  /* Check for errors. */
  if (End == Str || *End != '\0')
  { /* Error. */
    ErrNo = EINVAL;
  } /* Error. */

  else if (ErrNo == 0)
  { /* No error, so return converted value. */
    *Val = NewVal;
  } /* No error, so return converted value. */

  /* Restore global 'errno' and return 'ErrNo'. */
  errno = SavedErrNo;
  return ErrNo;
} /* StrToChar(char *, char *) */
