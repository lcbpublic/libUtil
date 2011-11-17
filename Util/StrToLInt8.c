#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Equivalent to strtoil8() but with simplified error reporting.
 * Returns the errno value rather than setting the global errno.  The
 * global errno is always left unchanged.  It is an error not to use
 * all of 'Str'.  Returns ERANGE on overflow/underflow, EINVAL for a
 * malformed 'Str', and EDOM for an invalid 'Base'.  If the function
 * returns non-zero then '*Val' is unchanged.  In all cases neither
 * 'Str' nor 'Val' may be NULL.
 *****************************************************************************/
int StrToLInt8(const char *Str, int Base, int_least8_t *Val)
{ /* StrToLInt8(char *, int, int_least8_t *) */
  char *End = NULL;
  int_least8_t NewVal;
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

  /* strtoi8() returns error status in 'errno' but we must leave it
   * unchanged. */
  SavedErrNo = errno;
  errno = 0;

  /* Do the conversion. */
  NewVal = strtoil8(Str, &End, Base);

  /* Save errno and restore original value. */
  ErrNo = errno;
  errno = SavedErrNo;

  /* Check for errors. */
  /* EINVAL is ambiguous but any other value we just return to the caller. */
  if (ErrNo != 0 && ErrNo != EINVAL)
  { /* Range error. */
    /* Do nothing. */
  } /* Range error. */

  /* ErrNo may or may not be EINVAL for an invalid Base so check explicitly. */
  else if (Base != 0 && (Base < 2 || Base > 36))
  { /* Invalid base. */
    ErrNo = EDOM;
  } /* Invalid base. */

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
} /* StrToLInt8(char *, int, int_least8_t *) */
