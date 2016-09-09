#include <errno.h>
#include <limits.h>
#include <ctype.h>

#include "StrToX.h"

/******************************************************************************
 * Equivalent to strtof() but with simplified error reporting.
 * Returns the 'errno' value rather than setting the global 'errno'.
 * The global 'errno' is always left unchanged.  It is an error not to
 * use all of 'Str', or for (*Str == '\0').  Returns ERANGE on
 * overflow/underflow and EINVAL for a malformed 'Str'.  If the
 * function returns non-zero then '*Val' is unchanged.  In all cases
 * neither 'Str' nor 'Val' may be NULL.
 *****************************************************************************/
int StrToF(const char *Str, float *Val)
{ /* StrToF(const char *, float *) */
  char *End;
  float NewVal;
  int SavedErrNo, ErrNo;

  /* Error checking. */
  if (Str == NULL || Val == NULL)
  { /* Error. */
    return EFAULT;
  } /* Error. */

  if (*Str == '\0' || isspace(*Str))
  { /* Error. */
    return EINVAL;
  } /* Error. */

  /* strtof() returns error status in 'errno' but we must leave it
   * unchanged. */
  SavedErrNo = errno;

  /* Do the conversion. */
  errno = 0;
  NewVal = strtof(Str, &End);
  ErrNo = errno;

  /****************************************************************************
   * We already know that 'Str' is not NULL, is not empty, and does
   * not begin with whitespace.  Therefore, we only have two error
   * cases to worry about:
   *
   * 1) 'Str' is malformed (but does not start with whitespace).  In
   *    this case strtof() will not have used all of 'Str' (i.e. (*End
   *    != '\0')) and we return EINVAL.
   *
   * 2) 'Str' is out of range.  In this case, strtof() sets 'errno' to
   *    ERANGE so all we have to do is skip returning the converted
   *    value and return 'ErrNo' (which we do anyway).
   *
   * Otherwise, the conversion was successful.
   *
   * NOTE: As I read both the Linux man pages and the POSIX.1-2008
   * specs, if 'Str' is entirely valid _except_ that it is out of
   * range, then (*endptr == '\0').  The Linux man page is silent on
   * the subject but the POSIX spec says: "The subject sequence is
   * defined as the longest initial subsequence of the input string,
   * starting with the first non-white-space character that is of the
   * expected form."  Therefore, at this point it is safe to first
   * check '*End' to see whether 'Str' is valid, then to check 'ErrNo'
   * to see if it was valid but out of range.
   ***************************************************************************/
  if (*End != '\0')
  { /* 'Str' is invalid. */
    ErrNo = EINVAL;
  } /* 'Str' is invalid. */

  else if (ErrNo == 0)
  { /* No error, so return converted value. */
    *Val = NewVal;
  } /* No error, so return converted value. */

  /* Restore global 'errno' and return 'ErrNo'. */
  errno = SavedErrNo;
  return ErrNo;
} /* StrToF(const char *, int, float *) */
