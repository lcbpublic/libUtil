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
#define RETURN(val) RETVAL = (val); goto Return
#define EXIT(val) RETVAL = (val); goto Exit

#endif
