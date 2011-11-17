#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Equivalent to strtouc() but with simplified error reporting.
 * Returns the errno value rather than setting the global errno.  The
 * global errno is always left unchanged.  It is an error not to use
 * all of 'Str'.  Returns ERANGE on overflow/underflow and EINVAL for
 * a malformed 'Str'.  If the function returns non-zero then '*Val' is
 * unchanged.  In all cases neither 'Str' nor 'Val' may be NULL.
 *****************************************************************************/
int StrToUChar(const char *Str, unsigned char *Val)
{ /* StrToUChar(char *, char *) */
  unsigned char NewVal;
  char *End;
  int SavedErrNo, ErrNo;

  /* Error checking. */
  if (Str == NULL || Val == NULL)
  { /* Error. */
    return EFAULT;
  } /* Error. */

  /* strtoc() returns error status in 'errno' but we must leave it
   * unchanged. */
  SavedErrNo = errno;
  errno = 0;

  /* Do the conversion. */
  NewVal = strtouc(Str, &End);

  /* Save errno and restore original value. */
  ErrNo = errno;
  errno = SavedErrNo;

  /* Check for errors. */
  if (End == Str || *End != '\0')
  { /* Error. */
    ErrNo = EINVAL;
  } /* Error. */

  /* Return results. */
  if (ErrNo == 0)
  { /* No error. */
    *Val = NewVal;
  } /* No error. */
  return ErrNo;
} /* StrToChar(char *, unsigned char *) */
