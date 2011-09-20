#ifndef Util_h
#define Util_h

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
  /* These interpret C escape sequences in Str. */
  extern char strtoc(const char *Str, char **End);
  extern signed char strtosc(const char *Str, char **End);
  extern unsigned char strtouc(const char *Str, char **End);

  /* These interpret Str as an eight bit integer. */
  extern int8_t strtob(const char *Str, char **End, int Base);
  extern uint8_t strtoub(const char *Str, char **End, int Base);

  /* These are all numeric. */
  extern short strtos(const char *Str, char **End, int Base);
  extern unsigned short strtous(const char *Str, char **End, int Base);
  extern int strtoi(const char *Str, char **End, int Base);
  extern unsigned int strtoui(const char *Str, char **End, int Base);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
