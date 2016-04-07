#include <errno.h>
#include <limits.h>

#include "StrToX.h"

/******************************************************************************
 * Function equivalent to strtol() but for 'short'.
 *
 * If 'short' and 'long' have the same range then we simply call
 * strtol() and cast the result to 'short'.
 *
 * Otherwise, we save the value of 'errno' on entry, call strtol() to
 * do the conversion, and clamp the result to the range of a 'short'
 * setting 'errno' to 'ERANGE' if necessary, or restoring the saved
 * 'errno' if no errors occurred.
 *
 * At first glance this seems too simplistic.  Shouldn't we check
 * 'errno' to detect errors?  However, as it turns out, the
 * Byzantine error semantics of 'strtol()' and friends make 'errno'
 * almost useless for our purposes.  To see this let's consider error
 * checking based on 'errno':
 *
 * switch (errno)
 * { // Check for errors.
 *   case 0:
 *     // No error in strtol().  However, 'strtol()' might succeed but
 *     // return a value out of range for a 'short'.  So, ...
 *     if (Val > SHRT_MAX)
 *     { // Overflow.
 *       Val = SHRT_MAX;
 *       errno = ERANGE;
 *     } // Overflow.
 *
 *     else if (Val < SHRT_MIN)
 *     { // Underflow.
 *       Val = SHRT_MIN;
 *       errno = ERANGE;
 *     } // Underflow.
 *
 *     else
 *     { // No error ... really.  Restore saved value.
 *       errno = SavedErrNo;
 *     } // No error ... really.  Restore saved value.
 *     break;
 *
 *   case ERANGE:
 *     // If the value is out of range for a 'long' then it's certainly
 *     // out of range for a 'short'.  However, we must return either
 *     // SHRT_MAX or SHRT_MIN.
 *     if (Val > SHRT_MAX)
 *       Val = SHRT_MAX;
 *     else if (Val < SHRT_MIN)
 *       Val = SHRT_MIN;
 *     break;
 *
 *   case EINVAL:
 *     // Oooohh, tricky!  Plain ISO C never specifies a case where 'errno'
 *     // is set to EINVAL.  However, since the function's behavior is
 *     // completely undefined if 'base' is invalid, 'errno' _might_ be set
 *     // to EINVAL in that case.  In that case we also need to make sure the
 *     // return value is in range for a 'short'.  On the other hand, in
 *     // all cases where POSIX.1-2008 specifies 'errno' "shall" or "may" be
 *     // set to EINVAL, the function returns zero.  If no conversion is
 *     // performed then, additionally, (*endptr == nptr).  We simply want
 *     // to preserve this behavior in either ISO or POSIX.  So, ...
 *     if (Val > SHRT_MAX)
 *       Val = SHRT_MAX;
 *     else if (Val < SHRT_MIN)
 *       Val = SHRT_MIN;
 *     break;
 *
 *   default:
 *     // We have some undefined behavior.  Just make sure 'Val' is in
 *     // range for a 'short'.
 *     if (Val > SHRT_MAX)
 *       Val = SHRT_MAX;
 *     else if (Val < SHRT_MIN)
 *       Val = SHRT_MIN;
 *     break;
 * } // Check for errors.
 *
 * Hunh.  All we ever do is clamp the return value to the range of a
 * 'short' and leave everything else as is except for restoring
 * 'errno' if there was no error.  So, ...
 *****************************************************************************/
short strtos(const char *Str, char **End, int Base)
{ /* strtos(const char *, char **, int) */
#if SHRT_MAX == LONG_MAX && SHRT_MIN == LONG_MIN
  /* In the event that 'short' and 'long' are the same size just call
   * strtol() and cast the result. */
  return (short) strtol(Str, End, Base);

#elif SHRT_MAX < LONG_MAX && SHRT_MIN > LONG_MIN
  int SavedErrNo;
  long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long'. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Clamp the converted value to the range of a 'short'.  If 'errno'
   * is not already set, set it to 'ERANGE' */
  if (Val > SHRT_MAX)
  { /* Overflow. */
    Val = SHRT_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (Val < SHRT_MIN)
  { /* Underflow. */
    Val = SHRT_MIN;
    if (errno == 0)
      errno = ERANGE;
  } /* Underflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (short) Val;
#else
#error "Can not implement strtos()."
#endif
} /* strtos(const char *, char **, int) */
