#ifndef Errors_h
#define Errors_h

/******************************************************************************
 * Stuff for handling errors.  For the moment, all we do is define a
 * few handy macros.  Ultimately, we'd like to have a full blown error
 * handling "class"es.
 *****************************************************************************/
/******************************************************************************
 * This macro is pretty self explanatory.  We save the value 'val' in
 * the variable 'var' (actually, 'var' can be any valid 'lhs'), then
 * 'goto' the label 'label'.  NOTE: If 'val' is intended to be the
 * function return value, the programmer is responsible for making
 * sure 'var' actually gets returned at some point.
 *****************************************************************************/
#define GOTO(var, val, label) (var) = (val); goto label

/******************************************************************************
 * These two are specialized versions of 'GOTO' above.  The variable
 * which holds the return value is defined as '_RetVal'.  The macro
 * 'RETURN' assigns 'val' to '_RetVal' then jumps to label 'Return'.
 * 'EXIT' does the same but jumps to 'Exit'.  This allows "normal" and
 * "error" exits to do different things.
 *****************************************************************************/
#ifndef RETVAL
#define RETVAL _RetVal
#endif
#define RETURN(val) RETVAL = (val); goto Return
#define EXIT(val) RETVAL = (val); goto Exit

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
extern const char *StrError(int ErrNo);

#endif
