#include <errno.h>
#include <limits.h>

#include "Util.h"

/******************************************************************************
 * Functions equivalent to strtol(), etc., for other basic types.
 *****************************************************************************/
signed char strtosc(const char *Str, char **End)
{ /* strtosc(char *, char **) */
  const char *Src;
  signed char Char;

  Src = Str;
  switch (*Src)
  { /* Select character. */
    case '\0':
    { /* Error. */
      if (End != NULL)
        *End = (char *) Str;
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
        { /* '\x' */
          /* Make sure we start with a hex digit. If not, then what we
           * have a problem.  In GCC a sting like "\x stuff" generates
           * an error because "\x" _must_ be followed by at least one
           * hex digit so the conversion must "fail".  There are
           * several ways we might indicate failure but we elect to do
           * the following.  We return zero and set '*End' to 'Str'.
           * This seems equivalent to, say, strtol() which, if no
           * digits are found, returns zero and sets '*endptr' to
           * 'nptr'.  We also elect _not_ to set 'errno' to EINVAL
           * since, on Linux at least, EINVAL seems to be reserved for
           * an invalid base in integer conversions.  */
          ++Src;
          if (!isxdigit(*Src))
          { /* Not hex digit. */
            Char = 0;
            Src = Str;
          } /* Not hex digit. */
          else
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

            /*
             * Convert to unsigned long.  Note: The hex escape code
             * syntax (\x<hdigit>[<hdigit>]) does not allow for
             * negative values so we convert to unsigned long then
             * cast to char when we're done.
             */
            errno = 0;
            HexVal = strtoul(Src, &EndPtr, 16);
            Src = EndPtr;

            /* Check for errors. */
            if (HexVal > UCHAR_MAX)
            { /* Out of range. */
              HexVal = SCHAR_MAX;
              if (errno == 0)
                errno = ERANGE;
            } /* Out of range. */

            if (errno == 0)
            { /* No errors. */
              errno = SavedErrNo;
            } /* No errors. */

            Char = (signed char) HexVal;
          } /* Hexidecimal character code. */
        } /* '\x' */
        break;

        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        { // Octal character code.
          char *EndPtr;
          int SavedErrNo;
          unsigned long OctVal;

          /* No need to check for an octal character since our 'case'
           * statements guarantee we have one. */

          /*
           * If we are successful we must not modify errno.  However,
           * to reliably detect errors in strtoul() we must set errno
           * to zero.  So, ...
           */
          SavedErrNo = errno;

          /*
           * Convert to unsigned long.  Note that the octal escape
           * code syntax (\<odigit>[<odigit>]) does not allow for
           * negative values.  So, we must convert to unsigned then
           * cast to char when we're done.
           */
          errno = 0;
          OctVal = strtoul(Src, &EndPtr, 8);
          Src = EndPtr;

          /* Check for errors. */
          if (OctVal > UCHAR_MAX)
          { /* Out of range. */
            OctVal = SCHAR_MAX;
            if (errno == 0)
              errno = ERANGE;
          } /* Out of range. */

          if (errno == 0)
          { /* No errors. */
            errno = SavedErrNo;
          } /* No errors. */

          Char = (signed char) OctVal;
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
} /* strtosc(char *, char **) */
