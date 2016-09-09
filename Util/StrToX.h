#ifndef StrToX_h
#define StrToX_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

/******************************************************************************
 * Functions for converting strings to various numeric types.  The
 * 'strtoX()' functions are equivalent to 'strtol()', etc., for other
 * basic types with the exceptions of 'strtochar()', 'strtoschar()',
 * and 'strtouchar()'.  The latter three return the first "character"
 * from 'Str' including characters repreesented by escape sequences.
 *
 * The StrToX() functions do the same conversions but with greatly
 * simplified (and restrictive) error semantics.
 *****************************************************************************/
/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *
 * WARNING: All the 'strtoX()' functions except 'strtochar()',
 * 'strtoschar()', and 'strtouchar()' (which we wrote from scratch)
 * are in turn based on 'strtol()', 'strtoul()', 'strtoll()', and
 * 'strtoull()'.  These underlying functions have error semantics
 * which are at best awkward, and at worst undefined.  Read both the
 * Linux man pages, and the Open Group's "The Single Unix
 * Specification" / POSIX.1-2008 spec carefully(!) as the
 * corresponding functions declared here ('strtos()', etc.) have
 * exactly analogous error semantics.
 *****************************************************************************/
  /* These interpret C escape sequences in 'Str'.  NOTE: These
   * functions do _not_ understand the '\unnnn' and '\Unnnn' escape
   * sequences since these can translate to a multi-char sequence and
   * we only return a single character.  */
  extern char strtochar(const char *Str, char **End);
  extern signed char strtoschar(const char *Str, char **End);
  extern unsigned char strtouchar(const char *Str, char **End);

  /* These are all numeric.  WARNING: Make sure you really know what
   * you're doing before using 'strtoc()'.  Since 'char' may be either
   * signed or unsigned, it may behave like either 'strtosc()' or
   * 'strtouc()' depending. */
  static inline char strtoc(const char *Str, char **End, int Base);
  extern signed char strtosc(const char *Str, char **End, int Base);
  extern unsigned char strtouc(const char *Str, char **End, int Base);
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
 * semantics.  They are:
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
 *   2.4) For functions that take a 'Base' argument, if '(Base != 0 &&
 *        (Base < 2 || Base > 36))', then EDOM is returned.
 *
 *   2.5) Should the underlying 'strtoX()' function sets 'errno' to any
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
  /* These interpret C escape sequences in 'Str'.  NOTE: These
   * functions do _not_ understand the '\unnnn' and '\Unnnn' escape
   * sequences since these can translate to a multi-char sequence and
   * we only return a single character.  */
  extern int StrToChar(const char *Str, char *Val);
  extern int StrToSChar(const char *Str, signed char *Val);
  extern int StrToUChar(const char *Str, unsigned char *Val);

  /* These are all numeric.  WARNING: Make sure you really know what
   * you're doing before using 'StrToChar()'.  Since 'char' may be
   * either signed or unsigned, it may behave like either
   * 'StrToSChar()' or 'StrToUChar()' depending. */
  static inline int StrToC(const char *Str, int Base, char *Val);
  extern int StrToSC(const char *Str, int Base, signed char *Val);
  extern int StrToUC(const char *Str, int Base, unsigned char *Val);
  extern int StrToS(const char *Str, int Base, short *Val);
  extern int StrToUS(const char *Str, int Base, unsigned short *Val);
  extern int StrToI(const char *Str, int Base, int *Val);
  extern int StrToUI(const char *Str, int Base, unsigned int *Val);
  extern int StrToL(const char *Str, int Base, long *Val);
  extern int StrToUL(const char *Str, int Base, unsigned long *Val);
  extern int StrToLL(const char *Str, int Base, long long *Val);
  extern int StrToULL(const char *Str, int Base, unsigned long long *Val);

  extern int StrToF(const char *Str, float *Val);
  extern int StrToD(const char *Str, double *Val);
  extern int StrToLD(const char *Str, long double *Val);

  /* Conversions for types defined in stdint.h.  WARNING: These should
   * work on platforms where a 'long' is at least 32 bits and a 'long
   * long' is at least 64 bits.  All bets are off for other cases */
  extern int StrToI8(const char *Str, int Base, int8_t *Val);
  extern int StrToUI8(const char *Str, int Base, uint8_t *Val);
  extern int StrToI16(const char *Str, int Base, int16_t *Val);
  extern int StrToUI16(const char *Str, int Base, uint16_t *Val);
  extern int StrToI32(const char *Str, int Base, int32_t *Val);
  extern int StrToUI32(const char *Str, int Base, uint32_t *Val);
  extern int StrToI64(const char *Str, int Base, int64_t *Val);
  extern int StrToUI64(const char *Str, int Base, uint64_t *Val);
  extern int StrToIMax(const char *Str, int Base, intmax_t *Val);
  extern int StrToUMax(const char *Str, int Base, uintmax_t *Val);

  extern int StrToIL8(const char *Str, int Base, int_least8_t *Val);
  extern int StrToUIL8(const char *Str, int Base, uint_least8_t *Val);
  extern int StrToIL16(const char *Str, int Base, int_least16_t *Val);
  extern int StrToUIL16(const char *Str, int Base, uint_least16_t *Val);
  extern int StrToIL32(const char *Str, int Base, int_least32_t *Val);
  extern int StrToUIL32(const char *Str, int Base, uint_least32_t *Val);
  extern int StrToIL64(const char *Str, int Base, int_least64_t *Val);
  extern int StrToUIL64(const char *Str, int Base, uint_least64_t *Val);

  extern int StrToIF8(const char *Str, int Base, int_fast8_t *Val);
  extern int StrToUIF8(const char *Str, int Base, uint_fast8_t *Val);
  extern int StrToIF16(const char *Str, int Base, int_fast16_t *Val);
  extern int StrToUIF16(const char *Str, int Base, uint_fast16_t *Val);
  extern int StrToIF32(const char *Str, int Base, int_fast32_t *Val);
  extern int StrToUIF32(const char *Str, int Base, uint_fast32_t *Val);
  extern int StrToIF64(const char *Str, int Base, int_fast64_t *Val);
  extern int StrToUIF64(const char *Str, int Base, uint_fast64_t *Val);

/******************************************************************************
 * Inline method definitions.
 *****************************************************************************/
  static inline char strtoc(const char *Str, char **End, int Base)
  { /* strtoc() */
#if CHAR_MIN == SCHAR_MIN && CHAR_MAX == SCHAR_MAX
    return (char) strtosc(Str, End, Base);
#elif CHAR_MIN == UCHAR_MIN && CHAR_MAX == UCHAR_MAX
    return (char) strtouc(Str, End, Base);
#else
#error "Can not implement strtoc()."
#endif
  } /* strtoc() */

  static inline int StrToC(const char *Str, int Base, char *Val)
  { /* StrToC() */
#if CHAR_MIN == SCHAR_MIN && CHAR_MAX == SCHAR_MAX
    return StrToSC(Str, Base, (signed char *) Val);
#elif CHAR_MIN == UCHAR_MIN && CHAR_MAX == UCHAR_MAX
    return StrToUC(Str, Base, (unsigned char *) Val);
#else
#error "Can not implement StrToChar()."
#endif
  } /*StrToC() */

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
