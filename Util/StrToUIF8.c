#include <errno.h>
#include <limits.h>
#include <ctype.h>

#include "StrToX.h"

/******************************************************************************
 * Equivalent to strtouif8() but with simplified error reporting.
 * Returns the 'errno' value rather than setting the global 'errno'.
 * The global 'errno' is always left unchanged.  It is an error not to
 * use all of 'Str', or for (*Str == '\0').  Returns ERANGE on
 * overflow/underflow, EINVAL for a malformed 'Str', EDOM for an
 * invalid 'Base', and EFAULT if either 'Str' or 'Val' is NULL.  If
 * the function returns non-zero then '*Val' is unchanged.
 *****************************************************************************/
int StrToUIF8(const char *Str, int Base, uint_fast8_t *Val)
{ /* StrToUIF8(const char *, int, uint_fast8_t *) */
  char *End;
  uint_fast8_t NewVal;
  int SavedErrNo, ErrNo;

  /* NULL pointer checking. */
  if (Str == NULL || Val == NULL)
  { /* Error. */
    return EFAULT;
  } /* Error. */

  /* Check that 'Str' is not empty, nor does it start with white space
   * both of which strtouif8() considers valid. */
  if (*Str == '\0' || isspace(*Str))
  { /* Error. */
    return EINVAL;
  } /* Error. */

  /* We don't rely on strtouif8() to check for an invalid base because,
   * depending on standards conformance, in this case it's behavior
   * may be undefined. */
  if (Base != 0 && (Base < 2 || Base > 36))
  { /* Invalid base. */
    return EDOM;
  } /* Invalid base. */

  /****************************************************************************
   * strtouif8() silently converts negative values to unsigned so we
   * need to check whether 'Str' represents a negative number
   * (i.e. whether the number part of 'Str' starts with '-').  The
   * specs/man pages say that for strtoul(), and friends the only
   * thing that can validly precede the number is white space but we
   * just checked for that.  So we now know that '*Str' is either a
   * '+', a '-', a digit, or an invalid character.  All of these
   * except '-' will be handled correctly by strtouif8() so it is
   * sufficient to check that (*Str != '-').
   ***************************************************************************/
  if (*Str == '-')
  { /* Negative number. */
    return ERANGE;
  } /* Negative number. */
  
  /* strtouif8() returns error status in 'errno' but we must leave it
   * unchanged. */
  SavedErrNo = errno;

  /* Do the conversion. */
  errno = 0;
  NewVal = strtouif8(Str, &End, Base);
  ErrNo = errno;

  /****************************************************************************
   * We already know that 'Str' is not NULL, is not empty, and does
   * not begin with whitespace.  We also know that 'Base' is valid.
   * Therefore, we only have two error cases to worry about:
   *
   * 1) 'Str' is malformed (but does not start with whitespace).  In
   *    this case strtouif8() will not have used all of 'Str' (i.e. (*End
   *    != '\0')) and we return EINVAL.
   *
   * 2) 'Str' is out of range.  In this case, strtouif8() sets 'errno' to
   *    ERANGE so all we have to do is skip returning the converted
   *    value and return 'ErrNo' (which we do anyway).
   *
   * Otherwise, the conversion was successful.
   *
   * NOTE: As I read both the Linux man pages and the POSIX.1-2008
   * specs, if 'Str' is entirely valid _except_ that it is out of
   * range, then (*endptr == '\0').  Specifically, from the man page:
   * "The remainder of the string is converted to a long int value in
   * the obvious manner, stopping at the first character which is not
   * a valid digit in the given base."  And from the POSIX spec: "The
   * subject sequence is defined as the longest initial subsequence of
   * the input string, starting with the first non-white-space
   * character that is of the expected form."  Therefore, at this
   * point it is safe to first check '*End' to see whether 'Str' is
   * valid, then to check 'ErrNo' to see if it was valid but out of
   * range.
   ***************************************************************************/
  if (*End != '\0')
  { /* Invalid Str. */
    ErrNo = EINVAL;
  } /* Invalid Str. */

  else if (ErrNo == 0)
  { /* No error, so return converted value. */
    *Val = NewVal;
  } /* No error, so return converted value. */

  /* Restore global 'errno' and return 'ErrNo'. */
  errno = SavedErrNo;
  return ErrNo;
} /* StrToUIF8(const char *, int, uint_fast8_t *) */
