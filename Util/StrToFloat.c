#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Equivalent to strtof() but with simplified error reporting.
 * Returns the errno value rather than setting the global errno.  The
 * global errno is always left unchanged.  It is an error not to use
 * all of 'Str'.  Returns ERANGE on overflow/underflow and EINVAL for
 * a malformed 'Str'.  If the function returns non-zero then '*Val' is
 * unchanged.  In all cases neither 'Str' nor 'Val' may be NULL.
 *****************************************************************************/
int StrToFloat(const char *Str, float *Val)
{ /* StrToFloat(char *, float *) */
  char *End;
  float NewVal;
  int SavedErrNo, ErrNo;

  /* Error checking. */
  if (Str == NULL || Val == NULL)
  { /* Error. */
    return EFAULT;
  } /* Error. */

  if (isspace(*Str))
  { /* Error. */
    return EINVAL;
  } /* Error. */

  /* strtos() returns error status in 'errno' but we must leave it
   * unchanged. */
  SavedErrNo = errno;
  errno = 0;

  /* Do the conversion. */
  NewVal = strtof(Str, &End);

  /* Save errno and restore original value. */
  ErrNo = errno;
  errno = SavedErrNo;

  /* Check for errors. */
  /* EINVAL is ambiguous but any other value we just return to the caller. */
  if (ErrNo != 0 && ErrNo != EINVAL)
  { /* Range error. */
    /* Do nothing. */
  } /* Range error. */

  /* ErrNo may or may not be EINVAL for an invalid Str so check explicitly. */
  else if (*Str == '\0' || *End != '\0')
  { /* Invalid Str. */
    ErrNo = EINVAL;
  } /* Invalid Str. */

  /* Return results.  By this point ErrNo should only be zero if 'Str'
   * was a valid string. */
  if (ErrNo == 0)
  { /* No error. */
    *Val = NewVal;
  } /* No error. */
  return ErrNo;
} /* StrToFloat(char *, int, float *) */