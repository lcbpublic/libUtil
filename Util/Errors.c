/******************************************************************************
 * Functions to simplify and standardize error handling.
 *****************************************************************************/
#include <errno.h>
#include <locale.h>
#include <string.h>

#include <Errors.h>

/******************************************************************************
 * Get error message corresponding to a system 'errno' value.
 * Basically, a thread-safe version off 'strerror()'.
 *
 * All the POSIX and GNU/Linux variants of 'strerror()' have one or
 * more signficant falings:
 *
 * strerror(): Not thread-safe.
 *
 * strerror_r(): Requires the user to supply a buffer to hold the
 *   error message but there is no way to determine the longest
 *   possible error message and, consequently, the size of buffer to
 *   pass to 'strerror_r()'.  Furthermore, there are two common
 *   versions of 'strerror_r()' floating around: The POSIX version and
 *   the GNU version which have different semantics and return values.
 *   Making sure we get the right one and/or determining which one(s)
 *   are available requires messing with various "feature macros", a
 *   skill which I have yet to master.
 *
 * strerror_l(): Like 'strerror()' but thread-safe.  Unfortunately,
 *   also requires a 'locale_t' object.
 *
 * So, we provide 'StrError()' but otherwise behaves just like
 * 'strerror()' ... with the following exception: 'StrError()' is
 * based on 'strerror_l()' which, as noted above, requires a
 * 'locale_t` object.  We obtain a 'locale_t' for the current locale
 * by calling 'newlocale()' which may fail.  In this case 'StrError()'
 * returns an error message indicating that the (real) error message
 * could not be obtaind and why.
 *****************************************************************************/
const char *StrError(int ErrNo)
{ /* StrError() */
  static const char *LocEInvalMsg =
    "<No error message.  EINVAL getting locale.  Request for invalid locale data category, or invalid locale name.>";
  static const char *LocENoEntMsg =
    "<No error message.  ENOENT getting locale.  Locale not available.>";
  static const char *LocENoMemMsg =
    "<No error message.  ENOMEM getting locale.  Insufficient memory to create locale object.>";
  static const char *LocUnknownMsg = "<No error message.  Unknown error getting locale.>";
  static const char *NoMsgMsg = "<No error message.>";
  const char *Msg, *_RetVal;
  locale_t Locale;
  int SavedErrNo, LocalErrNo;

  /* Try to get the current locale. */
  SavedErrNo = errno;
  Locale = newlocale(LC_MESSAGES_MASK, "", (locale_t) 0);
  LocalErrNo = errno;
  if (Locale == (locale_t) 0)
  { /* Error. */
    switch (LocalErrNo)
    { /* Select error. */
      case EINVAL:
        Msg = LocEInvalMsg;
        break;

      case ENOENT:
        Msg = LocENoEntMsg;
        break;

      case ENOMEM:
        Msg = LocENoMemMsg;
        break;

      default:
        Msg = LocUnknownMsg;
        break;
    } /* Select error. */

    EXIT(Msg);
  } /* Error. */

  /* Get error message. */
  errno = 0;
  Msg = strerror_l(ErrNo, Locale);
  LocalErrNo = errno;
  if (Msg == NULL)
  { /* Error. */
    Msg = NoMsgMsg;
  } /* Error. */

  RETURN(Msg);
  
Exit:
Return:
  if (Locale != (locale_t) 0)
    freelocale(Locale);
  errno = SavedErrNo;
  return _RetVal;
} /* StrError() */
