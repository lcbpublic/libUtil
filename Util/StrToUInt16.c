#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Equivalent to strtoui16() but with simplified error reporting.
 * Returns the errno value rather than setting the global errno.  The
 * global errno is always left unchanged.  It is an error not to use
 * all of 'Str'.  Returns ERANGE on overflow/underflow, EINVAL for a
 * malformed 'Str', and EDOM for an invalid 'Base'.  If the function
 * returns non-zero then '*Val' is unchanged.  In all cases neither
 * 'Str' nor 'Val' may be NULL.
 *****************************************************************************/
int StrToUInt16(const char *Str, int Base, uint16_t *Val)
{ /* StrToUInt16(char *, int, uint16_t *) */
  char *End;
  uint16_t NewVal;
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

  /* strtoui16() returns error status in 'errno' but we must leave it
   * unchanged. */
  SavedErrNo = errno;
  errno = 0;

  /* Do the conversion. */
  NewVal = strtoui16(Str, &End, Base);

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

  /* By now we know the conversion was successful but strtoui16()
   * silently converts negatives to positives so we have to check
   * manually.  We want to accept '-0' however. */
  else if (*Str == '-' && NewVal != 0)
  { /* Error. */
    ErrNo = ERANGE;
  } /* Error. */

  /* Return results.  By this point ErrNo should only be zero if 'Str'
   * was a valid string. */
  if (ErrNo == 0)
  { /* No error. */
    *Val = NewVal;
  } /* No error. */
  return ErrNo;
} /* StrToUInt16(char *, int, uint16_t *) */
