#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
unsigned char strtouc(const char *Str, char **End)
{ /* strtouc(char *, char **) */
  const char *Src;
  unsigned char Char;

  if (Str == NULL)
  { /* Error. */
    errno = EINVAL;
    return 0;
  } /* Error. */
  Src = Str;

  switch (*Src)
  { /* Select character. */
    case '\0':
    { /* Error. */
      errno = EINVAL;
      return 0;
    } /* Error. */
    break;

    case '\\':
    { /* Escape sequence. */
      Src++;
      switch (*Src)
      { /* Select real char. */
        case 'a':
          Char = '\a';
          ++Src;
          break;

        case 'b':
          Char = '\b';
          ++Src;
          break;

        case 'f':
          Char = '\f';
          ++Src;
          break;

        case 'n':
          Char = '\n';
          ++Src;
          break;

        case 'r':
          Char = '\r';
          ++Src;
          break;

        case 't':
          Char = '\t';
          ++Src;
          break;

        case 'v':
          Char = '\v';
          ++Src;
          break;

        case '\\':
          Char = '\\';
          ++Src;
          break;

        case 'x':
        { /*  Hexidecimal character code. */
          char *EndPtr;
          int SavedErrNo;
          unsigned long HexVal;

          /* 
           * If we are successful we must not modify errno.  However,
           * to reliably detect errors in strtoul() we must set errno
           * to zero.  So, ...
           */
          SavedErrNo = errno;

          /* Make sure we start with a hex digit. */
          ++Src;
          if (!isxdigit(*Src))
          { // Error.
            errno = EINVAL;
            return (unsigned char) 0;
          } // Error.

          /*
           * Convert to unsigned long.  Note that the hex escape
           * code syntax (\x<hdigit>[<hdigit>]) does not allow for
           * negative values.  So, we must convert to unsigned then
           * cast to char when we're done.
           */
          errno = 0;
          HexVal = strtoul(Src, &EndPtr, 16);
          Src = EndPtr;
          if (errno != 0)
          { /* Error. */
            if (HexVal > UCHAR_MAX)
              HexVal = UCHAR_MAX;
          } /* Error. */

          /* Check range. */
          else if (HexVal > UCHAR_MAX)
          { /* Range error. */
            HexVal = UCHAR_MAX;
            errno = ERANGE;
          } /* Range error. */

          else
            errno = SavedErrNo;

          Char = (unsigned char) HexVal;
        } /* Hexidecimal character code. */
        break;

        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        { // Octal character code.
          char *EndPtr;
          int SavedErrNo;
          unsigned long OctVal;

          /*
           * If we are successful we must not modify errno.  However,
           * to reliably detect errors in strtoul() we must set errno
           * to zero.  So, ...
           */
          SavedErrNo = errno;

          /*
           * Convert to unsigned long.  Note that the hex escape
           * code syntax (\x<hdigit>[<hdigit>]) does not allow for
           * negative values.  So, we must convert to unsigned then
           * cast to char when we're done.
           */
          errno = 0;
          OctVal = strtoul(Src, &EndPtr, 8);
          Src = EndPtr;

          if (errno != 0)
          { /* Error. */
            if (OctVal > UCHAR_MAX)
              OctVal = UCHAR_MAX;
          } /* Error. */

          /* Check range. */
          else if (OctVal > UCHAR_MAX)
          { /* Range error. */
            OctVal = UCHAR_MAX;
            errno = ERANGE;
          } /* Range error. */

          else
            errno = SavedErrNo;

          Char = (unsigned char) OctVal;
        } /* Octal character code. */
        break;

        default:
        { /* Everything else. */
          Char = *Src++;
        } /* Everything else. */
        break;
      } /* Select real char. */
    } /* Escape sequence. */
    break;

    default:
      Char = *Src++;
      break;
  } /* Select character. */

  if (End != NULL)
    *End = (char *) Src;
  return Char;
} /* strtouc(char *, char **) */
