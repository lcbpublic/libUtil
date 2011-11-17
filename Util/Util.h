#ifndef Util_h
#define Util_h

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.  In
 * all cases 'Str' _must_ not be NULL.
 *
 * WARNING: All the strtox() functions except strtoc(), strtosc() and
 * strtouc() are in turn based on strtol(), strtoul(), strtoll(), or
 * strtoull().  As such they share the underlying function's error
 * semantics which are, sadly, poorly defined.  Considering both the
 * Linux man pages, the Open Group's "The Single Unix Specification",
 * and http://www.austingroupbugs.net/view.php?id=453 it appears we
 * can only count on the following:
 *
 *   1) On success 'errno' is unchanged and (endptr != nptr).
 *
 *   2) If the value is out of range for the return type (errno ==
 *      ERANGE) and (endptr != nptr).
 *
 *   3) If 'base' is valid but no conversion is performed (endptr ==
 *      nptr).  Also, 'errno' _may_ be EINVAL.
 *
 *   4) If 'base' is invalid [sigh] behavior is basically undefined.
 *      However, according to empirical testing (see
 *      http://www.austingroupbugs.net/view.php?id=453) (errno ==
 *      EINVAL) and 'endptr' is undefined.
 *
 * So, if 'errno' is set to zero before the call then the following
 * should work:
 *
 *   val = strtol(Str, &End, Base);
 *
 *   // Errno is always ERANGE for range errors.
 *   if (errno == ERANGE)
 *     <Range error>
 *
 *   // We should never get any other value for errno besides EINVAL.
 *   else if (errno != 0 && errno != EINVAL)
 *     <Unexpected error>
 *
 *   // Errno may or may not be EINVAL for an invalid base so check explicitly.
 *   else if (Base != 0 && (Base < 2 || Base > 36))
 *     <Invalid base>
 *
 *   // Errno may or may not be EINVAL for an invalid Str so check explicitly
 *   // but End is always set to Str.
 *   else if (End == Str)
 *     <Str invalid>
 *
 *   // We successfully parsed a number.
 *   else
 *     <Success>
 *
 * What a mess!
 *****************************************************************************/
  /* These interpret C escape sequences in Str. */
  extern char strtoc(const char *Str, char **End);
  extern signed char strtosc(const char *Str, char **End);
  extern unsigned char strtouc(const char *Str, char **End);

  /* These are all numeric. */
  extern short strtos(const char *Str, char **End, int Base);
  extern unsigned short strtous(const char *Str, char **End, int Base);
  extern int strtoi(const char *Str, char **End, int Base);
  extern unsigned int strtoui(const char *Str, char **End, int Base);

  /* Conversions for types defined in stdint.h.  WARNING: These should
   * work on platforms where a 'long' is at least 32 bits and a 'long
   * long' is at least 64 bits.  All bets are off for other cases */
  extern int8_t strtoi8(const char *Str, char **End, int Base);
  extern uint8_t strtoui8(const char *Str, char **End, int Base);
  extern int16_t strtoi16(const char *Str, char **End, int Base);
  extern uint16_t strtoui16(const char *Str, char **End, int Base);
  extern int32_t strtoi32(const char *Str, char **End, int Base);
  extern uint32_t strtoui32(const char *Str, char **End, int Base);
  extern int64_t strtoi64(const char *Str, char **End, int Base);
  extern uint64_t strtoui64(const char *Str, char **End, int Base);

  extern int_least8_t strtoil8(const char *Str, char **End, int Base);
  extern uint_least8_t strtouil8(const char *Str, char **End, int Base);
  extern int_least16_t strtoil16(const char *Str, char **End, int Base);
  extern uint_least16_t strtouil16(const char *Str, char **End, int Base);
  extern int_least32_t strtoil32(const char *Str, char **End, int Base);
  extern uint_least32_t strtouil32(const char *Str, char **End, int Base);
  extern int_least64_t strtoil64(const char *Str, char **End, int Base);
  extern uint_least64_t strtouil64(const char *Str, char **End, int Base);

  extern int_fast8_t strtoif8(const char *Str, char **End, int Base);
  extern uint_fast8_t strtouif8(const char *Str, char **End, int Base);
  extern int_fast16_t strtoif16(const char *Str, char **End, int Base);
  extern uint_fast16_t strtouif16(const char *Str, char **End, int Base);
  extern int_fast32_t strtoif32(const char *Str, char **End, int Base);
  extern uint_fast32_t strtouif32(const char *Str, char **End, int Base);
  extern int_fast64_t strtoif64(const char *Str, char **End, int Base);
  extern uint_fast64_t strtouif64(const char *Str, char **End, int Base);

/******************************************************************************
 * Functions equivalent to strtol(), etc. above but with simplified
 * error reporting.  These functions return the errno value rather
 * than setting the global errno.  The global errno is always left
 * unchanged.  It is an error not to use all of 'Str' and leading
 * white-space is not allowed.  On overflow/underflow they return
 * ERANGE.  For a malformed 'Str' they return EINVAL.  For an invalid
 * 'Base' (where applicable) they return EDOM.  If either 'Str' or
 * 'Val' are NULL they return EFAULT.  Other values may be returned by
 * the underlying 'strtox()' functions although none are currently
 * documented.  If any of these functions returns non-zero then '*Val'
 * is unchanged.
 *****************************************************************************/
  /* These interpret C escape sequences in Str. */
  extern int StrToChar(const char *Str, char *Val);
  extern int StrToSChar(const char *Str, signed char *Val);
  extern int StrToUChar(const char *Str, unsigned char *Val);

  /* These are all numeric. */
  extern int StrToShort(const char *Str, int Base, short *Val);
  extern int StrToUShort(const char *Str, int Base, unsigned short *Val);
  extern int StrToInt(const char *Str, int Base, int *Val);
  extern int StrToUInt(const char *Str, int Base, unsigned int *Val);
  extern int StrToLong(const char *Str, int Base, long *Val);
  extern int StrToULong(const char *Str, int Base, unsigned long *Val);
  extern int StrToLongLong(const char *Str, int Base, long long *Val);
  extern int StrToULongLong(const char *Str, int Base, unsigned long long *Val);

  extern int StrToFloat(const char *Str, float *Val);
  extern int StrToDouble(const char *Str, double *Val);
  extern int StrToLongDouble(const char *Str, long double *Val);

#if 0
  /* Conversions for types defined in stdint.h.  WARNING: These should
   * work on platforms where a 'long' is at least 32 bits and a 'long
   * long' is at least 64 bits.  All bets are off for other cases */
  extern int StrToInt8(const char *Str, int Base, int8_t *Val);
  extern int StrToUInt8(const char *Str, int Base, uint8_t *Val);
  extern int StrToInt16(const char *Str, int Base, int16_t *Val);
  extern int StrToUInt16(const char *Str, int Base, uint16_t *Val);
  extern int StrToInt32(const char *Str, int Base, int32_t *Val);
  extern int StrToUInt32(const char *Str, int Base, uint32_t *Val);
  extern int StrToInt64(const char *Str, int Base, int64_t *Val);
  extern int StrToUInt64(const char *Str, int Base, uint64_t *Val);

  extern int StrToLInt8(const char *Str, int Base, int_least8_t *Val);
  extern int StrToULInt8(const char *Str, int Base, uint_least8_t *Val);
  extern int StrToLInt16(const char *Str, int Base, int_least16_t *Val);
  extern int StrToULInt16(const char *Str, int Base, uint_least16_t *Val);
  extern int StrToLInt32(const char *Str, int Base, int_least32_t *Val);
  extern int StrToULInt32(const char *Str, int Base, uint_least32_t *Val);
  extern int StrToLInt64(const char *Str, int Base, int_least64_t *Val);
  extern int StrToULInt64(const char *Str, int Base, uint_least64_t *Val);

  extern int StrToFInt8(const char *Str, int Base, int_fast8_t *Val);
  extern int StrToUFInt8(const char *Str, int Base, uint_fast8_t *Val);
  extern int StrToFInt16(const char *Str, int Base, int_fast16_t *Val);
  extern int StrToUFInt16(const char *Str, int Base, uint_fast16_t *Val);
  extern int StrToFInt32(const char *Str, int Base, int_fast32_t *Val);
  extern int StrToUFInt32(const char *Str, int Base, uint_fast32_t *Val);
  extern int StrToFInt64(const char *Str, int Base, int_fast64_t *Val);
  extern int StrToUFInt64(const char *Str, int Base, uint_fast64_t *Val);
#endif
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
