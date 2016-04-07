#include <errno.h>
#include <limits.h>

#include "StrToX.h"

/******************************************************************************
 * Function equivalent to strtoul() but for 'unsigned short'.
 *
 * If 'unsigned short' and 'unsigne long' have the same range then we
 * simply call strtoul() and cast the result to 'unsigned short'.
 *
 * Otherwise, we save the value of 'errno' on entry, call strtol() to
 * do the conversion, clamp the result to the range of a 'unsigned
 * short', and sett 'errno' to ERANGE if necessary, or restoring the
 * saved value if no error occurred.
 *
 * Why call strtol() instead of strtoul()?  strtoul() silently
 * converts negatives to positives so '-1' is indistinguishable from
 * 0xffffffffffffffff.  By analogy with strtoul(), the former should
 * be acceptable to strtous() (and should return ((unsigned short)
 * -1)) while the latter should return USHRT_MAX and set 'errno' to
 * ERANGE.  The only way I can see to do this is to do a _signed_
 * conversion using strtol() then if (Val < -USHRT_MAX || Val >
 * USHRT_MAX) set 'errno' to ERANGE and return USHRT_MAX.  This is
 * admittedly nasty but I don't see any other good way of doing it.
 *
 * Furthermore, at first glance it would seem that simply clamping the
 * return value to the range of an 'unsigned short' is too simplistic.
 * Shouldn't we check 'errno' to detect errors?  However, as it turns
 * out, the Byzantine error semantics of 'strtol()' and friends make
 * 'errno' almost useless for our purposes.  To see this let's
 * consider error checking based on 'errno':
 *
 * switch (errno)
 * { // Check for errors.
 *   case 0:
 *     // No error ... maybe.  'strtol()' might succeed but return a value
 *     // out of range for an 'unsigned short'.  So, ...
 *     if (Val > USHRT_MAX || Val < -USHRT_MAX)
 *     { // Overflow.
 *       Val = SHRT_MAX;
 *       errno = ERANGE;
 *     } // Overflow.
 *
 *     else
 *     { // No error ... really.  Restore saved value.
 *       errno = SavedErrNo;
 *     } // No error ... really.  Restore saved value.
 *     break;
 *
 *   case ERANGE:
 *     // If the value is out of range for a 'long' then it's certainly
 *     // out of range for an 'unsigned short'.  However, we must return
 *     // USHRT_MAX.
 *     Val = USHRT_MAX;
 *     break;
 *
 *   case EINVAL:
 *     // Oooohh, tricky!  Plain ISO C never specifies a case where 'errno'
 *     // is set to EINVAL.  However, since the function's behavior is
 *     // completely undefined if 'base' is invalid, 'errno' _might_ be set
 *     // to EINVAL in that case.  In that case we also need to make sure the
 *     // return value is in range for an 'unsigned short'.  On the other hand, 
 *     // in all cases where POSIX.1-2008 specifies 'errno' "shall" or "may" be
 *     // set to EINVAL, the function returns zero.  If no conversion is
 *     // performed then, additionally, (*endptr == nptr).  We simply want
 *     // to preserve this behavior in either ISO or POSIX.  So, ...
 *     if (Val > USHRT_MAX || Val < -USHRT_MAX)
 *       Val = USHRT_MAX;
 *     break;
 *
 *   default:
 *     // We have some undefined behavior.  Just make sure 'Val' is in
 *     // range for a 'short'.
 *     if (Val > USHRT_MAX || Val < -USHRT_MAX)
 *       Val = USHRT_MAX;
 *     break;
 * } // Check for errors.
 *
 * Hunh.  All we ever do is clamp the return value to the range of an
 * 'unsigned short' and leave everything else as is except for
 * restoring 'errno' if there was no error.  So, ...
 *****************************************************************************/
unsigned short strtous(const char *Str, char **End, int Base)
{ /* strtous(const char *, char **, int) */
#if USHRT_MAX == ULONG_MAX
  /* In the event that 'unsigned short' and 'unsigned long' are the
   * same size just call strtoul() and cast the result. */
  return (unsigned short) strtoul(Str, End, Base);

#elif USHRT_MAX < LONG_MAX
  /* If a 'long' can represent an 'unsigned short' then call
   * strtol() and check the result.  See comments below for more
   * details. */
  int SavedErrNo;
  long Val;

  /* Save initial value of 'errno' */
  SavedErrNo = errno;

  /* Convert to 'long'. */
  errno = 0;
  Val = strtol(Str, End, Base);

  /* Clamp the converted value to the range of an 'unsigned short'.
   * If 'errno' is not already set, set it to 'ERANGE' */
  if (Val > (long) USHRT_MAX || Val < -((long) USHRT_MAX))
  { /* Overflow. */
    Val = USHRT_MAX;
    if (errno == 0)
      errno = ERANGE;
  } /* Overflow. */

  else if (errno == 0)
  { /* No other error so restore saved 'errno'. */
    errno = SavedErrNo;
  } /* No other error so restore saved 'errno'. */

  /* Return the result. */
  return (unsigned short) Val;
#else
#error "Can not implement strtous()."
#endif
} /* strtous(const char *, char **, int) */
