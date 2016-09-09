#ifndef String_h
#define String_h

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * A String 'class'.  The buffer containing the string is resized
 * dynamially as required.  We implement the "small string
 * optiization" so he minimum buffer size is 'STRING_MIN_BUFF_SIZE'
 * (currently '#define'ed' as 16 in 'String.h').  When growing the
 * buffer the resize factor is 2.  We never voluntarily shrink the
 * buffer, although 'StringResizeToFit()' can be called to force the
 * buffer to be resized to exactly contain the current string
 * (altthough, because of the "small string optiization" it will never
 * be less than STRING_MIN_BUFF_SIZE).
 *****************************************************************************/
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * The 'struct' that holds the data for our 'class'.
 *****************************************************************************/
#define STRING_MIN_BUFF_SIZE 16

struct String
{ /* String */
  /* The size (_not_ including the terminating '\0') of the current contents
   * of 'Buff'. */
  size_t Size;

  /*
   * Okay, this is tricky.  I first read about this here:
   * http://info.prelert.com/blog/cpp-stdstring-implementations .  If
   * 'Ptr' points 'Buff.Str' then we know a priori what the buffer
   * size is (i.e. STRING_MIN_BUFF_SIZE) so we don't actually need it
   * and we can use 'Buff.Str' to store the sting (no malloc required
   * but 'Buff.Size' is invalid).  On the other hand, if 'Ptr' points
   * somewhere else, then 'Buff.Size' _does_ hold the buffer size (and
   * 'Buff.Str' is invalid).  So, ...
   */
  union {
    size_t Size;
    char Data[STRING_MIN_BUFF_SIZE];
  } Buff;

  /* Pointer to the string/buffer. */
  char *Ptr;
}; /* String */
typedef struct String String_t;

/******************************************************************************
 * These would be 'public' member functions in C++.  All
 * 'ConstructStringX()' functions return 'true' on success.  On
 * failure they call 'StringDesstroy(This), set 'errno', and return
 * 'false'.  All 'NewStringX()' functions return a pointer to a newly
 * created and initialized 'struct String' on success.  On failure
 * they destry and 'free()' any partialy ceated 'struct String', set
 * 'errno', and return NULL.  Return values for other functions are
 * documented below.  NOTE: If 'NDEBUG' is defined, then a certain
 * amount of error checking is eliminated.  In particular, passing
 * NULL pointers will most likely lead to a segfault or some such.
 *****************************************************************************/
/* Default constructor.  We pre-allocate an eight 'char' buffer (seven
 * characters plus the terminating '\0'). */
static inline bool ConstructString(String_t *This)
{ /* ConstructString() */
#ifndef NDEBUG
  if (This == NULL)
  { /* Error. */
    printf("%s %s %d: Internal error.  'This' is NULL.\n",
           __FILE__, __func__, __LINE__);
    errno = EFAULT;
    return false;
  } /* Error. */
#endif
  
  This->Ptr = This->Buff.Data;
  This->Ptr[0] = '\0';
  This->Size = 0;

  return true;
} /* ConstructString() */
extern String_t *NewString();

/* Copy construstor. */
extern bool ConstructStringStr(String_t *This, const String_t *Src);
extern String_t *NewStringStr(const String_t *Src);

/* Construct a 'String_t' from a 'const char *'. */
extern bool ConstructStringCStr(String_t *This, const char *Src);
extern String_t *NewStringCStr(const char *Src);

/* Construct a 'String_t' containing 'Count' repetitions of 'Char'. */
extern bool ConstructStringChar(String_t *This, char Char, size_t Count);
extern String_t *NewStringChar(char Char, size_t Count);

/* Destructor. */
extern void DestroyString(String_t *This);
static inline void DeleteString(String_t *This)
{ /* DeleteString() */
  if (This != NULL)
  { /* Exists. */
    DestroyString(This);
    free(This);
  } /* Exists. */
} /* DeleteString() */

/* Make buffer at least 'MinSize' characters long. */
extern bool StringReserve(String_t *This, size_t MinSize);

static inline size_t StringCapacity(String_t *This)
{ /* Capacity() */
  return (This->Ptr == This->Buff.Data ?
          STRING_MIN_BUFF_SIZE : This->Buff.Size);
} /* Capacity() */

/* Resize the buffer to exactly contain the current contents including
 * the terminating NUL. */
extern bool StringResizeToFit(String_t *This);

/* Equivalent to 'StringSetCStr(This, "")' except guaranteed not to
 * resize the buffer. */
static inline void StringClear(String_t *This)
{ /* StringClear() */
  This->Ptr[0] = '\0';
  This->Size = 0;
} /* StringClear() */

/*
 * Returns 'This' if successful, NULL otherwise.  On error, 'errno' is
 * set to non-zero (probably ENOMEM).
 */
extern bool StringSetCStr(String_t *This, const char *Src);

/* Get the current contents as a 'const char *'.  WARNING: The
 * returned pointer may become invalid after any operation the changes
 * the size of the string! */
static inline const char *StringGetCStr(String_t *This) {
  return This->Ptr;
}

/*
 * Returns 'This' if successful, NULL otherwise.  On error, 'errno' is
 * set to non-zero (probably ENOMEM).
 */
extern bool StringSetStr(String_t *This, const String_t *Src);

/* Append a 'const char *' to 'This'. */
extern bool StringAppendCStr(String_t *This, const char *Str);

/* Append a 'const String_t *' to 'This'. */
extern bool StringAppendStr(String_t *This, const String_t *Str);

/* Append a 'const char *' to 'This' returning the result in a new
 * 'String_t'.  Returns NULL and sets 'errno' on error. */
extern String_t *
StringConcatCStr(String_t *Str, const char *CStr);

/* Append a 'String_t *' to 'This' returning the result in a new
 * 'String_t'.  Returns NULL and sets 'errno' on error. */
extern String_t *
StringConcatStr(String_t *Str0, const String_t *Str1);

/* Analogous to sprintf(). */
extern int StringPrintf(String_t *This, const char *Fmt, ...);

/* Like 'StringPrintf()' but appends output to the current contents of
 * 'This'.  Makes the 'This' vaguely like a 'std::ostringstream'
 * except missing all the file-like funcctionality (i.e. 'seek()',
 * 'putback()', etc..). */
int StringStreamf(String_t *This, const char *Fmt, ...);
#endif
