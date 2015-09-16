#ifndef Util_h
#define Util_h

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Functions for converting strings to various numeric types.  The
 * strtoX() functions are equivalent to strtol(), etc., for other
 * basic types.  The StrToX() functions do the conversions wqith
 * greatly simplified error semantics.
 *****************************************************************************/
/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *
 * WARNING: All the strtox() functions except strtoc(), strtosc() and
 * strtouc() (which we wrote from scratch) are in turn based on
 * strtol(), strtoul(), strtoll(), strtoull(), strtof(), strtod(), or
 * strtold().  These underlying functions have error semantics which
 * are at best awkward, and at worst undefined.  Read both the Linux
 * man pages, and the Open Group's "The Single Unix Specification" /
 * POSIX.1-2008 spec carefully(!) as the corresponding functions
 * declared here (strtos(), etc.) have exactly analogous error
 * semantics.
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
 * The StrToX() functions we provide below have much simpler error
 * semantics.  They are simply:
 *
 * 1) On success, the functions return 0 and the converted value is
 *    returned in '*Val'.
 *
 * 2) On error an 'errno' value is returned and '*Val' is unchanged.
 *
 *   2.1) If ('Str' == NULL || Val == NULL) then EFAULT is returned.
 *
 *   2.2) All of 'Str' must be used during the conversion (except the
 *        terminarting '\0').  If not, or if (*Str == '\0'), then
 *        EINVAL is returned.
 *
 *   2.3) If the value represented by 'Str' is out of range for the
 *        required type (including passing negative numbers to
 *        StrToUX()), then ERANGE is returned.
 *
 *   2.4) For functions that take a 'Base' argument, if (Base != 0 &&
 *        (Base < 2 || Base > 36)), then EDOM is returned.
 *
 *   2.5) Should the underlying strtox() function set 'errno' to any
 *        value other than 0, EINVAL, or ERANGE, that value is
 *        returned and '*Val' is unchanged.
 *
 * 3) On return, the global 'errno' is unchanged (although it may be
 *    temporarily modified internally to the functiuon).
 *
 * So, the caller simply needs to do (for exmple):
 *
 *   char *Str;
 *   short Val;
 *   int ErrNo;
 *
 *   // Get 'Str' from somewhere.
 *
 *   if ((ErrNo = StrToShort(Str, 0, &Val)) != 0)
 *   { // Handle error.
 *     ...
 *   } // Handle error.
 *
 *   // Use Val.
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
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
