/******************************************************************************
 * Function definitions for our 'struct String' "class".
 *****************************************************************************/
#include "Errors.h"
#include "String.h"

/******************************************************************************
 * These would be 'private' in C++.  WARNING: Programmer beware!  We
 * do minimal error checking here.  We assume that the 'public'
 * interface has already checked for NULL pointer arguments, etc..  We
 * still check for errors that can occur within the function
 * (e.g.'malloc()' returning NULL, etc.) but that's all.
 *****************************************************************************/
static inline bool IsLocalBuff(String_t *This)
{ /* IsLocalBuff() */
  return (This->Ptr == This->Buff.Data);
} /* IsLocalBuff() */

static inline size_t BuffSize(String_t *This)
{ /* BuffSize() */
  return (IsLocalBuff(This) ? STRING_MIN_BUFF_SIZE : This->Buff.Size);
} /* BuffSize() */

/******************************************************************************
 * Change the size of a String's buffer to be 'NewSize' unless
 * '(NewSize <= STRING_MIN_BUFF_SIZE)' in which case 'Buff.Data' is
 * used and the new size is 'STRING_MIN_BUFF_SIZE'.  NOTE: A buffer
 * allocated n the heap is obtained by calling 'malloc()' so the
 * contents of the new buffer are undefined on return!  The caller is
 * responsible for making sure it contains a valid string, and for
 * setting 'Size' correctly!  If 'malloc()' fails we return 'false'
 * and leave the current buffer in place.  Otherwise we return 'true'.
 *****************************************************************************/
static bool MallocBuffExact(String_t *This, size_t NewSize)
{ /* MallocBuffExact() */
  bool _RetVal;
  int ErrNo;
  size_t CurSize;
  char *NewBuff;

  /* Get the current buffer size.  NOTE: '(CurSize >=
   * STRING_MIN_BUFF_SIZE)' is always true. */
  CurSize = BuffSize(This);

  /* Make sure 'NewSize' isn't too small. */
  if (NewSize < STRING_MIN_BUFF_SIZE)
    NewSize = STRING_MIN_BUFF_SIZE;
  
  /* See if we actually are changing the size. */
  if (NewSize == CurSize)
  { /* Nothing to do. */
    RETURN(true);
  } /* Nothing to do. */
  
  /*
   * Okay, we're definitely changing the size.  If '(NewSize >
   * STRING_MIN_BUFF_SIZE)' then we have to call 'malloc()' to get the
   * new buffer (and possibly 'free()' to get rid of the old one).  If
   * '(NewSize == STRING_MIN_BUFF_SIZE)' (at this point it can't be
   * less than that) then the old buffer must be bigger than that so
   * we call 'free()' to get rid of it and set 'Buff.Data' as our new
   * buffer.  We return 'true' unless 'malloc()' fails.
   */
  if (NewSize > STRING_MIN_BUFF_SIZE)
  { /* Need to call 'malloc()' */
    if ((NewBuff = malloc(NewSize)) == NULL)
    { /* Error. */
      ErrNo = errno;
      printf("%s %s %d: malloc(%zu) failed.  %s.\n",
             __FILE__, __func__, __LINE__, NewSize, StrError(ErrNo));
      EXIT(false);
    } /* Error. */
    if (!IsLocalBuff(This))
      free(This->Ptr);
    This->Ptr = NewBuff;
    This->Buff.Size = NewSize;
  } /* Need to call 'malloc()' */
  else
  { /* Need to use 'Buff.Data'. */
    /* Since we're here we know the following: '(CurSize >
     * STRING_MIN_BUFF_SIZE && NewSize == STRING_MIN_BUFF_SIZE)'.
     * Therefore, we can 'free(Buff->Ptr)' without checking. */
    free(This->Ptr);
    This->Ptr = This->Buff.Data;
  } /* Need to use 'Buff.Data'. */
  RETURN(true);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* MallocBuffExact() */

/******************************************************************************
 * Change the size of a String's buffer to be 'NewSize' unless
 * '(NewSize <= STRING_MIN_BUFF_SIZE)' in which case 'Buff.Data' is
 * used and the new size is 'STRING_MIN_BUFF_SIZE'.  The contents of
 * the old buffer are preserved ... more-or-less.  If the new buffer
 * is smaller than the current _string_, then the string is truncated
 * to fit in the new buffer (including a terminating NUL).  If either
 * 'malloc()' or 'realloc()' fails we return 'false' and leave the
 * current buffer in place.  Otherwise we return 'true'.
 *****************************************************************************/
static bool ReallocBuffExact(String_t *This, size_t NewSize)
{ /* ReallocBuffExact() */
  bool _RetVal;
  int ErrNo;
  size_t CurSize;
  char *NewBuff;

  /* Get the current buffer size.  NOTE: '(CurSize >=
   * STRING_MIN_BUFF_SIZE)' is always true. */
  CurSize = BuffSize(This);

  /* Make sure 'NewSize' isn't too small. */
  if (NewSize < STRING_MIN_BUFF_SIZE)
    NewSize = STRING_MIN_BUFF_SIZE;

  /* See if we are actually changing the size. */
  if (NewSize == CurSize)
  { /* Nothing to do. */
    RETURN(true);
  } /* Nothing to do. */
  
  /*
   * Okay, we're definitely changing the size.  If '(NewSize >
   * STRING_MIN_BUFF_SIZE)' then we have to call either 'malloc()' or
   * 'realloc()' to get a new buffer (and possibly 'free()' to get rid
   * of the old one).  If '(NewSize == STRING_MIN_BUFF_SIZE)' (it
   * can't be less than that at this point) then, if necesary, we call
   * 'free()' to get rid of the old buffer and set 'Buff.Data' as our
   * new buffer.  We return 'true' unless 'malloc()' or 'realloc()'
   * fails.
   */
  if (NewSize > STRING_MIN_BUFF_SIZE)
  { /* Need to call 'malloc()' or 'realloc()' depending. */
    if (IsLocalBuff(This))
    { /* Currently using the local buffer so we have to call 'malloc()'. */
      if ((NewBuff = malloc(NewSize)) == NULL)
      { /* Error. */
        ErrNo = errno;
        printf("%s %s %d: malloc(%zu) failed.  %s.\n",
               __FILE__, __func__, __LINE__, NewSize, StrError(ErrNo));
        EXIT(false);
      } /* Error. */

      /* We know the new buffer is bigger so copy the string and leave
       * 'Size' unchanged. */
      memcpy(NewBuff, This->Ptr, This->Size);
    } /* Currently using the local buffer.  Use 'malloc()'. */
    else
    { /* Not using the local buffer. */
      if ((NewBuff = realloc(This->Ptr, NewSize)) == NULL)
      { /* Error. */
        ErrNo = errno;
        printf("%s %s %d: realloc(%zu) failed.  %s.\n",
               __FILE__, __func__, __LINE__, NewSize, StrError(ErrNo));
        EXIT(false);
      } /* Error. */

      /* 'realloc()' copied the contents for us, but it may not be
       * NUL terminated if NewSize is too small. */
      if (NewSize <= This->Size)
      { /* Truncated. */
        NewBuff[NewSize-1] = '\0';
        This->Size = NewSize - 1;
      } /* Truncated. */
    } /* Not using the local buffer. */

    This->Ptr = NewBuff;
    This->Buff.Size = NewSize;
  } /* Need to call 'malloc()' or 'realloc()' depending. */
  else
  { /* Need to use 'Buff.Data'. */
    /* Copy all the data that will fit, from the current buffer to the
     * local buffer (a la 'realloc()'). */
    memcpy(This->Buff.Data, This->Ptr, NewSize);

    /* See if we truncated the string.  If so, NUL terminate it and
     * set 'This->Size' accordingly. */
    if (NewSize <= This->Size)
    { /* Truncated. */
      This->Buff.Data[NewSize-1] = '\0';
      This->Size = NewSize - 1;
    } /* Truncated. */

    /* Since we're here we know the following: '(CurSize >
     * STRING_MIN_BUFF_SIZE && NewSize == STRING_MIN_BUFF_SIZE)'.
     * Therefore, we can 'free(Buff->Ptr)' without checking. */
    free(This->Ptr);
    This->Ptr = This->Buff.Data;
  } /* Need to use 'Buff.Data'. */
  RETURN(true);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* ReallocBuffExact() */

/******************************************************************************
 * Change the size of a String's buffer to be at least 'MinSize'.  The
 * contents of the new buffer are undefined!  The caller is
 * responsible for making sure the buffer contains a valid string.
 *
 * If '(MinSize > BuffSize(This))', then we call
 * 'MallocBuffExact()' to allocate a new buffer that is a power
 * of 2 times the current size.  If '!IsLocalBuff(This)' then we
 * 'free()' the existing buffer.
 *
 * If 'STRING_ALLOW_SHRINK' is defined and '(MinSize < BuffSize(This)
 * / 4 && MinSize > STRING_MIN_BUFF_SIZE))', then we call
 * 'MallocBuffExact()' to allocate a new buffer.  If '(MinSize <
 * BuffSize(This) && MinSize <= STRING_MIN_BUFF_SIZE)' then we set the
 * buffer to 'This->Buff.Data'.  We 'free()' the existing buffer.
 *
 * In all other cases we do nothing.
 *
 * If 'MallocBuffExact()' fails we return 'false' and leave the current buffer
 * in place.  Otherwise we return 'true'.
 *****************************************************************************/
static bool MallocBuff(String_t *This, size_t MinSize)
{ /* MallocBuff() */
  bool _RetVal;
  int ErrNo;
  size_t CurSize, NewSize, Temp;

  /* Get the current buffer size. */
  CurSize = BuffSize(This);

  /* Grow buffer. */
  if (MinSize > CurSize)
  { /* Grow buffer. */
    /* We have to be careful here because, in principle, multiplying
     * by 2 might overflow. */
    NewSize = CurSize;
    while (NewSize < MinSize)
    { /* While not big enough. */
      if ((Temp = NewSize << 1) < NewSize)
      { /* Overflow. */
        /* Okay.  If we overflowed then the next biggest value we can
         * get is the max value of a 'size_t' (i.e. SIZE_MAX).  Since
         * 'MinSize' is also a 'size_t' then '(SIZE_MAX >= 'MinSize)'
         * must also be true.  So, we're also done. */
        NewSize = SIZE_MAX;
      } /* Overflow. */
      else
      { /* No overflow. */
        NewSize = Temp;
      } /* No overflow. */
    } /* While not big enough. */

    /* Get new buffer. */
    if (!MallocBuffExact(This, NewSize))
    { /* Error. */
      ErrNo = errno;
      printf("%s %s %d: MallocBuffExact(%zu) failed.  %s.\n",
             __FILE__, __func__, __LINE__, NewSize, StrError(ErrNo));
      EXIT(false);
    } /* Error. */
  } /* Grow buffer. */

#ifdef STRING_ALLOW_SHRINK  
  else if ((NewSize = CurSize >> 2) >= MinSize)
  { /* Shrink buffer. */
    /* Start with 'CurSize / 4' since we already know 'MinSize' is
     * less than or equal to that. */
    while ((Temp = NewSize >> 1) > MinSize)
    { /* While 'NewSize >> 1' still bigger than requested size. */
      NewSize = Temp;
    } /* While 'NewSize >> 1' still bigger than requested size. */
    
    if (Temp == MinSize)
    { /* If 'NewSize << 1' equals requested size, then use that. */
      NewSize = Temp;
    } /* If 'NewSize << 1' equals requested size, then use that. */
    
    /* Get new buffer. */
    if (!MallocBuffExact(This, NewSize))
    { /* Error. */
      ErrNo = errno;
      printf("%s %s %d: MallocBuffExact(%zu) failed.  %s.\n",
             __FILE__, __func__, __LINE__, NewSize, StrError(ErrNo));
      EXIT(false);
    } /* Error. */
  } /* Shrink buffer. */
#endif
  RETURN(true);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* MallocBuff() */

/******************************************************************************
 * Change the size of a String's buffer to be at least 'MinSize'.  The
 * contents of the old buffer may be _not_ preserved!  If '(MinSize >
 * CurSize || (MinSize < CurSize / 4 && MinSize >
 * STRING_MIN_BUFF_SIZE))', then we call 'malloc()' to allocate a new
 * one.  If '(MinSize <= STRING_MIN_BUFF_SIZE)' then we set the buffer
 * to 'This->Buff.Data'.  Otherwie we do nothing.  If 'malloc()' fails
 * we return 'false' and leave the current buffer in place.
 * Otherwise we return 'true'.
 *****************************************************************************/
static bool ReallocBuff(String_t *This, size_t MinSize)
{ /* ReallocBuff() */
  bool _RetVal;
  int ErrNo;
  size_t CurSize, NewSize, Temp;

  /* Get the current buffer size. */
  CurSize = BuffSize(This);

  /* See what we need to do. */
  if (MinSize > CurSize)
  { /* Grow buffer. */
    /* We have to be careful here because, in principle, multiplying
     * by 2 might overflow. */
    NewSize = CurSize;
    while (NewSize < MinSize)
    { /* While not big enough. */
      if ((Temp = NewSize << 1) <= NewSize)
      { /* Overflow. */
        /* Okay.  If we overflowed then the next biggest value we can
         * get is the max value of a 'size_t' (i.e. SIZE_MAX).  Since
         * 'MinSize' is also a 'size_t' then '(SIZE_MAX >= 'MinSize'
         * must also be true.  So, we're also done. */
        NewSize = SIZE_MAX;
      } /* Overflow. */
      else
      { /* No overflow. */
        NewSize = Temp;
      } /* No overflow. */
    } /* While not big enough. */

    /* Get new buffer. */
    if (!ReallocBuffExact(This, NewSize))
    { /* Error. */
      ErrNo = errno;
      printf("%s %s %d: ReallocBuffExact(%zu) failed.  %s.\n",
             __FILE__, __func__, __LINE__, NewSize, StrError(ErrNo));
      EXIT(false);
    } /* Error. */
  } /* Grow buffer. */

#ifdef STRING_ALLOW_SHRINK  
  else if ((NewSize = CurSize >> 2) >= MinSize)
  { /* Shrink buffer. */
    /* Start with 'CurSize / 4' since we already know 'MinSize' is
     * less than or equal to that. */
    while ((Temp = NewSize >> 1) > MinSize)
    { /* While 'NewSize >> 1' still bigger than requested size. */
      NewSize = Temp;
    } /* While 'NewSize >> 1' still bigger than requested size. */
    
    if (Temp == MinSize)
    { /* If 'NewSize << 1' equals requested size, then use that. */
      NewSize = Temp;
    } /* If 'NewSize << 1' equals requested size, then use that. */
    
    /* Get new buffer. */
    if (!ReallocBuffExact(This, NewSize))
    { /* Error. */
      ErrNo = errno;
      printf("%s %s %d: ReallocBuff(%zu) failed.  %s.\n",
             __FILE__, __func__, __LINE__, NewSize, StrError(ErrNo));
      EXIT(false);
    } /* Error. */
  } /* Shrink buffer. */
#endif
  RETURN(true);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* ReallocBuff() */

/******************************************************************************
 * These would be 'public' in C++.
 *****************************************************************************/
/* Default dynamic constructor. */
String_t *NewString()
{ /* NewString() */
  int ErrNo;
  String_t *This, *_RetVal;

  if ((This = malloc(sizeof(String_t))) == NULL)
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: malloc(%zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, sizeof(String_t),
           StrError(ErrNo));
    EXIT(NULL);
  } /* Error. */

  /* Default constructor.  We know this never fails if '(This != NULL)'. */
  ConstructString(This);
  RETURN(This);

Exit:
  free(This);
  errno = ErrNo;
  
Return:
  return _RetVal;
} /* NewString() */

/* Copy construstor. */
bool ConstructStringStr(String_t *This, const String_t *Src)
{ /* ConstructStringStr() */
  int ErrNo;
  bool Init = false, _RetVal;

  /* Error checking. */
  if (This == NULL)
  { /* Error. */
    printf("%s %s %d: 'This' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */
    
  if (Src == NULL)
  { /* Error. */
    printf("%s %s %d: 'Src' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */
    
  /* Default constructor.  Never fails if '(This != NULL)'. */
  Init = ConstructString(This);

  /* Assign 'Src' as our value. */
  if (!MallocBuff(This, Src->Size + 1))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: MallocBuff(This, %zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, Src->Size + 1,
           StrError(ErrNo));
    EXIT(false);
  } /* Error. */
  memcpy(This->Ptr, Src->Ptr, Src->Size + 1);
  This->Size = Src->Size;
  RETURN(true);

Exit:
  if (Init) DestroyString(This);
  errno = ErrNo;

Return:
  return _RetVal;
} /* ConstructStringStr() */

String_t *NewStringStr(const String_t *Src)
{ /* NewStringStr() */
  int ErrNo;
  String_t *This = NULL, *_RetVal;

  if ((This = malloc(sizeof(String_t))) == NULL)
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: malloc(%zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, sizeof(String_t),
           StrError(ErrNo));
    EXIT(NULL);
  } /* Error. */

  if (!ConstructStringStr(This, Src))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: StringConstuctStr() failed.  %s.\n",
           __FILE__, __func__, __LINE__, StrError(ErrNo));
    EXIT(NULL);
  } /* Error. */
  RETURN(This);

Exit:
  free(This);
  errno = ErrNo;

Return:
  return _RetVal;
} /* NewStringStr() */

bool ConstructStringCStr(String_t *This, const char *Src)
{ /* ConstructStringCStr() */
  int ErrNo;
  bool Init = false, _RetVal;
  size_t SrcSize;
  
  /* Error checking. */
  if (This == NULL)
  { /* Error. */
    printf("%s %s %d: 'This' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */

  if (Src == NULL)
  { /* Error. */
    printf("%s %s %d: 'Src' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */
    
  /* Default consstructor. This never fails if '(This != NULL)'. */
  Init = ConstructString(This);

  /* Assign 'Src' as our value. */
  SrcSize = strlen(Src);
  if (!MallocBuff(This, SrcSize + 1))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: MallocBuff(This, %zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, SrcSize + 1, StrError(ErrNo));
    EXIT(false);
  } /* Error. */
  memcpy(This->Ptr, Src, SrcSize + 1);
  This->Size = SrcSize;
  RETURN(true);

Exit:
  if (Init) DestroyString(This);
  errno = ErrNo;

Return:
  return _RetVal;
} /* ConstructStringCStr() */

String_t *NewStringCStr(const char *Src)
{ /* NewStringCStr() */
  int ErrNo;
  String_t *This = NULL, *_RetVal;

  if ((This = malloc(sizeof(String_t))) == NULL)
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: malloc(%zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, sizeof(String_t),
           StrError(ErrNo));
    EXIT(NULL);
  } /* Error. */

  if (!ConstructStringCStr(This, Src))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: StringConstuctCStr() failed.  %s.\n",
           __FILE__, __func__, __LINE__, StrError(ErrNo));
    EXIT(NULL);
  } /* Error. */
  RETURN(This);

Exit:
  free(This);
  errno = ErrNo;

Return:
  return _RetVal;
} /* NewStringCStr() */

bool ConstructStringChar(String_t *This, char Char, size_t Count)
{ /* ConstructStringChar() */
  bool Init = false, ErrNo, _RetVal;

  /* Error checking. */
  if (This == NULL)
  { /* Error. */
    printf("%s %s %d: 'This' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */
  
  /* We know by construction this never fails if '(This != NULL)'. */
  Init = ConstructString(This);

  /* Make sure 'Count' is not too big.  We need space for the
   * terminating NUL. */
  if (Count == SIZE_MAX)
  { /* Error. */
    printf("%s %s %d: Overflow.  'Count' is SIZE_MAX.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = ERANGE;
    EXIT(false);
  } /* Error. */
    
  /* Make sure our buffer is big enough. */
  if (!MallocBuff(This, Count + 1))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: 'MallocBuff(This, %zu)' failed.\n",
           __FILE__, __func__, __LINE__, Count + 1);
    EXIT(false);
  } /* Error. */
  /* Assign 'Char' as our value. */

  /* Build the string. */
  memset(This->Ptr, Char, Count);
  This->Ptr[Count] = '\0';
  This->Size = Count;
  RETURN(true);
  
Exit:
  if (Init) DestroyString(This);
  errno = ErrNo;

Return:
  return _RetVal;
} /* ConstructStringChar() */

String_t *NewStringChar(char Char, size_t Count)
{ /* NewStringChar() */
  int ErrNo;
  String_t *This = NULL, *_RetVal;

  if ((This = malloc(sizeof(String_t))) == NULL)
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: malloc(%zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, sizeof(String_t),
           StrError(ErrNo));
    EXIT(NULL);
  } /* Error. */

  if (!ConstructStringChar(This, Char, Count))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: ConstructStringChar(This, '%c', %zu) failed..\n",
           __FILE__, __func__, __LINE__, Char, Count);
    EXIT(NULL);
  } /* Error. */
  RETURN(This);

Exit:
  free(This);
  errno = ErrNo;
  
Return:
  return _RetVal;
} /* NewStringChar() */

/* Desructor. */
void DestroyString(String_t *This)
{ /* DestroyString() */
  if (This != NULL && !IsLocalBuff(This))
  { /* Exists, and not local buffer. */
    free(This->Ptr);
  } /* Exists, and not local buffer. */
} /* DestroyString() */

bool StringReserve(String_t *This, size_t NewSize)
{ /* StringReserve() */
  bool _RetVal;
  int ErrNo;

  /* Error checking */
  if (This == NULL)
  { /* Error. */
    printf("%s %s %d: 'This' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */

  if (!ReallocBuff(This, NewSize))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: ReallocBuff(This, %zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, NewSize, StrError(ErrNo));
    EXIT(false);
  } /* Error. */
  RETURN(true);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* StringReserve() */

bool StringResizeToFit(String_t *This)
{ /* StringResizeToFit() */
  bool _RetVal;
  int ErrNo;

  /* Error checking. */
  if (This == NULL)
  { /* Error. */
    printf("%s %s %d: 'This' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */

  /* Resize the buffer. */  
  if (!ReallocBuffExact(This, This->Size + 1))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: ReallocBuff(This, %zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, This->Size + 1, StrError(ErrNo));
    EXIT(false);
  } /* Error. */
  RETURN(true);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* StringResizeToFit() */

/*
 * Returns 'true' if successful, 'false' otherwise.  On error, 'errno' is
 * set to non-zero (probably ENOMEM).
 */
bool StringSetCStr(String_t *This, const char *Src)
{ /* StringSetCStr() */
  bool _RetVal;
  int ErrNo;
  size_t SrcSize;

  /* Error checking. */
  if (This == NULL)
  { /* Error. */
    printf("%s %s %d: 'This' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */

  if (Src == NULL)
  { /* Error. */
    printf("%s %s %d: 'Src' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */

  /* Get size of 'Src'.*/
  SrcSize = strlen(Src);

  /* Allocte a bigger buffer if we need to. */
  if (!MallocBuff(This, SrcSize + 1))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: MallocBuff(This, %zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, SrcSize + 1, StrError(ErrNo));
    EXIT(false);
  } /* Error. */

  /* Now we know we have enough space. */
  memcpy(This->Ptr, Src, SrcSize + 1);
  This->Size = SrcSize;
  RETURN(true);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* StringSetCStr() */

/*
 * Returns 'true' if successful, 'false' otherwise.  On error, 'errno' is
 * set to non-zero (probably ENOMEM).
 */
bool StringSetStr(String_t *This, const String_t *Src)
{ /* StringSetStr() */
  bool _RetVal;
  int ErrNo;
  
  /* Error checking. */
  if (This == NULL)
  { /* Error. */
    printf("%s %s %d: 'This' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */

  if (Src == NULL)
  { /* Error. */
    printf("%s %s %d: 'Src' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */

  /* Allocte a bigger buffer if we need to. */
  if (!MallocBuff(This, Src->Size + 1))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: MallocBuff(This, %zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, Src->Size + 1, StrError(ErrNo));
    EXIT(false);
  } /* Error. */

  /* Now we know we have enough space. */
  memcpy(This->Ptr, Src->Ptr, Src->Size + 1);
  This->Size = Src->Size;
  RETURN(true);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* StringSetStr() */

/*
 * Append a 'const char *' to 'This'.  Returns 'true' if successful,
 * 'false' otherwise.  On error, 'errno' is set to non-zero (probably
 * ENOMEM).
 */
bool StringAppendCStr(String_t *This, const char *Src)
{ /* StringAppendCStr() */
  bool _RetVal;
  int ErrNo;
  size_t SrcSize, NewSize;

  /* Error checking. */
  if (This == NULL)
  { /* Error. */
    printf("%s %s %d: 'This' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */

  if (Src == NULL)
  { /* Error. */
    printf("%s %s %d: 'Src' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */

  /* Compute new size.  As usual, we have to worry about overflow
   * detection/prevention. */
  SrcSize = strlen(Src);
  if (SIZE_MAX - (This->Size + 1) < SrcSize)
  { /* Result would overflow. */
    printf("%s %s %d: Size overflow.  This->Size: %zu  SrcSize: %zu.\n",
           __FILE__, __func__, __LINE__, This->Size, SrcSize);
    ErrNo = ERANGE;
    EXIT(false);
  } /* Result would overflow. */
  NewSize = This->Size + SrcSize;

  /* Allocte a bigger buffer if we need to. */
  if (!ReallocBuff(This, NewSize + 1))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: MallocBuff(This, %zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, SrcSize + 1, StrError(ErrNo));
    EXIT(false);
  } /* Error. */

  /* Now we know we have enough space. */
  memcpy(&This->Ptr[This->Size], Src, SrcSize + 1);
  This->Size = NewSize;
  RETURN(true);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* StringAppendCStr() */

/*
 * Append a 'const char *' to 'This'.  Returns 'true' if successful,
 * 'false' otherwise.  On error, 'errno' is set to non-zero (probably
 * ENOMEM).
 */
bool StringAppendStr(String_t *This, const String_t *Src)
{ /* StringAppendStr() */
  bool _RetVal;
  int ErrNo;
  size_t NewSize;

  /* Error checking. */
  if (This == NULL)
  { /* Error. */
    printf("%s %s %d: 'This' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */

  if (Src == NULL)
  { /* Error. */
    printf("%s %s %d: 'Src' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(false);
  } /* Error. */

  /* Compute new size.  As usual, we have to worry about overflow
   * detection/prevention. */
  if (SIZE_MAX - (This->Size + 1) < Src->Size)
  { /* Result would overflow. */
    printf("%s %s %d: Size overflow.  This->Size: %zu  Src->Size: %zu.\n",
           __FILE__, __func__, __LINE__, This->Size, Src->Size);
    ErrNo = ERANGE;
    EXIT(false);
  } /* Result would overflow. */
  NewSize = This->Size + Src->Size;

  /* Allocte a bigger buffer if we need to. */
  if (!ReallocBuff(This, NewSize + 1))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: MallocBuff(This, %zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, Src->Size + 1, StrError(ErrNo));
    EXIT(false);
  } /* Error. */

  /* Now we know we have enough space. */
  memcpy(&This->Ptr[This->Size], Src->Ptr, Src->Size + 1);
  This->Size = NewSize;
  RETURN(true);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* StringAppendStr() */

/*
 * Create a new 'String_t' containing 'Str' concatenated with
 * 'CStr'.  Returns the new 'struct Strng' if successful, NULL
 * otherwise.  On error, 'errno' is set to non-zero (probably ENOMEM).
 */
String_t *StringConcatCStr(String_t *Str, const char *CStr)
{ /* StringConcatCStr() */
  int ErrNo;
  size_t CStrSize, NewSize;
  String_t *NewStr = NULL, *_RetVal;

  /* Error checking. */
  if (Str == NULL)
  { /* Error. */
    printf("%s %s %d: 'Str' is NULL.\n", __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(NULL);
  } /* Error. */

  if (CStr == NULL)
  { /* Error. */
    printf("%s %s %d: 'CStr' is NULL.\n", __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(NULL);
  } /* Error. */

  /* Compute new size.  As usual, we have to worry about overflow
   * detection/prevention. */
  CStrSize = strlen(CStr);
  if (SIZE_MAX - (Str->Size + 1) < CStrSize)
  { /* Result would overflow. */
    printf("%s %s %d: Size overflow.  Str->Size: %zu  CStrSize: %zu.\n",
           __FILE__, __func__, __LINE__, Str->Size, CStrSize);
    ErrNo = ERANGE;
    EXIT(NULL);
  } /* Result would overflow. */
  NewSize = Str->Size + CStrSize;

  /* Create a new 'String_t' and 'Reserve' enough space for the result. */
  if ((NewStr = NewString()) == NULL)
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: StringNew() failed.  %s.\n",
           __FILE__, __func__, __LINE__, StrError(ErrNo));
    EXIT(NULL);
  } /* Error. */
  
  /* Allocte a bigger buffer if we need to. */
  if (!MallocBuff(NewStr, NewSize + 1))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: MallocBuff(Str, %zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, NewSize + 1, StrError(ErrNo));
    EXIT(NULL);
  } /* Error. */

  /* Now we know we have enough space. */
  memcpy(NewStr->Ptr, Str->Ptr, Str->Size);
  memcpy(&NewStr->Ptr[Str->Size], CStr, CStrSize + 1);
  NewStr->Size = NewSize;
  RETURN(NewStr);

Exit:
  DeleteString(NewStr);
  errno = ErrNo;
  
Return:
  return _RetVal;
} /* StringConcatCStr() */

/*
 * Create a new 'String_t' containing 'Str0' concatenated with
 * 'Str1'.  Returns the new 'struct String' if successful, NULL
 * otherwise.  On error, 'errno' is set to non-zero (probably ENOMEM).
 */
String_t *StringConcatStr(String_t *Str0, const String_t *Str1)
{ /* StringConcatStr() */
  int ErrNo;
  size_t NewSize;
  String_t *NewStr = NULL, *_RetVal;

  /* Error checking. */
  if (Str0 == NULL)
  { /* Error. */
    printf("%s %s %d: 'Str0' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(NULL);
  } /* Error. */

  if (Str1 == NULL)
  { /* Error. */
    printf("%s %s %d: 'Str1' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(NULL);
  } /* Error. */

  /* Compute new size.  As usual, we have to worry about overflow
   * detection/prevention. */
  if (SIZE_MAX - (Str0->Size + 1) < Str1->Size)
  { /* Result would overflow. */
    printf("%s %s %d: Size overflow.  Str0->Size: %zu  Str1Size: %zu.\n",
           __FILE__, __func__, __LINE__, Str0->Size, Str1->Size);
    ErrNo = ERANGE;
    EXIT(NULL);
  } /* Result would overflow. */
  NewSize = Str0->Size + Str1->Size;

  /* Create a new 'String_t' and 'Reserve' enough space for the result. */
  if ((NewStr = NewString()) == NULL)
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: NewString() failed.  %s.\n",
           __FILE__, __func__, __LINE__, StrError(ErrNo));
    EXIT(NULL);
  } /* Error. */
  
  /* Allocte a bigger buffer if we need to. */
  if (!MallocBuff(NewStr, NewSize + 1))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: MallocBuff(Str0, %zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, NewSize + 1, StrError(ErrNo));
    EXIT(NULL);
  } /* Error. */

  /* Now we know we have enough space. */
  memcpy(NewStr->Ptr, Str0->Ptr, Str0->Size);
  memcpy(&NewStr->Ptr[Str0->Size], Str1->Ptr, Str1->Size + 1);
  NewStr->Size = NewSize;
  RETURN(NewStr);

Exit:
  DeleteString(NewStr);
  errno = ErrNo;
  
Return:
  return _RetVal;
} /* StringConcatStr() */

/* Analogous to sprintf(). */
int StringPrintf(String_t *This, const char *Fmt, ...)
{ /* StringPrintf() */
  int FullSize, NewFullSize, _RetVal, ErrNo;
  size_t CurSize;
  va_list ap;

  /* Error checking. */
  if (This == NULL)
  { /* Error. */
    printf("%s %s %d: 'This' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(EOF);
  } /* Error. */

  if (Fmt == NULL)
  { /* Error. */
    printf("%s %s %d: 'Fmt' is NULL.\n",
           __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(EOF);
  } /* Error. */

  CurSize = BuffSize(This);
  
  va_start(ap, Fmt);
  FullSize = vsnprintf(This->Ptr, CurSize, Fmt, ap);
  va_end(ap);

  if (FullSize < 0)
  { /* Error (other than buffer overflow). */
    ErrNo = errno;
    
    /* We dunno what state the string is in so we clear it. */
    StringClear(This);

    printf("%s %s %d: vsnprint(This->Ptr, %zu, Fmt, ap) failed.  %s.\n",
           __FILE__,  __func__, __LINE__, CurSize, StrError(ErrNo));
    EXIT(FullSize);
  } /* Error (other than buffer overflow). */
  
  if (FullSize >= CurSize)
  { /* Ran out off space. */
    /* Make 'This' is consistent. */
    This->Ptr[CurSize-1] = '\0';

    /* Resize the buffer. */
    if (!MallocBuff(This, FullSize + 1))
    { /* Error. */
      ErrNo = errno;
      printf("%s %s %d: MallocBuff(This, %d) failed.  %s.\n",
             __FILE__,  __func__, __LINE__, FullSize + 1, StrError(ErrNo));
      EXIT(EOF);
    } /* Error. */

    /* Try with our new buffer. */
    va_start(ap, Fmt);
    NewFullSize = vsnprintf(This->Ptr, BuffSize(This), Fmt, ap);
    va_end(ap);

    if (NewFullSize < 0)
    { /* Error (other than buffer overflow). */
      ErrNo = errno;
      
      /* We dunno what state the string is in so we clear it. */
      StringClear(This);

      printf("%s %s %d: vsnprint(This->Ptr, %zu, Fmt, ap) failed.  %s.\n",
             __FILE__,  __func__, __LINE__, CurSize, StrError(ErrNo));
      EXIT(NewFullSize);
    } /* Error (other than buffer overflow). */

    if (NewFullSize != FullSize)
    { /* Error.  This should never happen! */
      /* Fix string. */
      This->Ptr[BuffSize(This)-1] = '\0';
      
      printf("%s %s %d: Size returned by 'vsnprintf(This->Ptr, %zu, Fmt, ap)'"
             " changed.  Was: %d  Is: %d"
             "  This should never happen!\n", __FILE__, __func__, __LINE__,
             BuffSize(This), FullSize, NewFullSize);
      ErrNo = ENOMEM;
      EXIT(EOF);
    } /* Error.  This should never happen! */
  } /* Ran out off space. */
  This->Size = FullSize;
  RETURN(FullSize);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* StringPrintf() */

/* Like 'StringPrintf()' but appends output to the current contents of
 * 'This'. */
int StringStreamf(String_t *This, const char *Fmt, ...)
{ /* StringStreamf() */
  int FullSize, NewFullSize, ErrNo, _RetVal;
  size_t CurSize;
  va_list ap;

  /* Error checking. */
  if (This == NULL)
  { /* Error. */
    printf("%s %s %d: 'This' is NULL.\n", __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(EOF);
  } /* Error. */

  if (Fmt == NULL)
  { /* Error. */
    printf("%s %s %d: 'Fmt' is NULL.\n", __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(EOF);
  } /* Error. */

  CurSize = BuffSize(This);
  
  va_start(ap, Fmt);
  FullSize = vsnprintf(&This->Ptr[This->Size], CurSize - This->Size, Fmt, ap);
  va_end(ap);

  if (FullSize < 0)
  { /* Error (other than buffer overflow). */
    ErrNo = errno;
    
    /* We dunno what happened so we return things to their previous state. */
    This->Ptr[This->Size] = '\0';

    printf("%s %s %d: 'vsnprint(&This->Ptr[%zu], %zu, Fmt, ap)' failed.  %s.\n",
           __FILE__,  __func__, __LINE__, This->Size, CurSize - This->Size,
           StrError(ErrNo));
    EXIT(FullSize);
  } /* Error (other than buffer overflow). */

  /* Add size of existing contents. */
  FullSize += This->Size;

  /* See whether we overflowed our current buffer. */
  if (FullSize >= CurSize)
  { /* Ran out off space. */
    /* Restore buffer to previous state. */
    This->Ptr[This->Size] = '\0';

    /* Resize the buffer. */
    if (!ReallocBuff(This, FullSize + 1))
    { /* Error. */
      ErrNo = errno;
      printf("%s %s %d: ReallocBuff(This, %d) failed.  %s.\n",
             __FILE__,  __func__, __LINE__, FullSize + 1, StrError(ErrNo));
      EXIT(EOF);
    } /* Error. */

    /* Try with our new buffer. */
    va_start(ap, Fmt);
    NewFullSize = vsnprintf(&This->Ptr[This->Size],
                            BuffSize(This) - This->Size, Fmt, ap);
    va_end(ap);

    if (NewFullSize < 0)
    { /* Error (other than buffer overflow). */
      ErrNo = errno;
      
      /* Restore string to previous state. */
      This->Ptr[This->Size] = '\0';

      printf("%s %s %d: 'vsnprint(This->Ptr, %zu, Fmt, ap)' failed.  %s.\n",
             __FILE__,  __func__, __LINE__, CurSize, StrError(ErrNo));
      EXIT(NewFullSize);
    } /* Error (other than buffer overflow). */
    NewFullSize += This->Size;
    
    if (NewFullSize != FullSize)
    { /* Error.  This should never happen! */
      /* Restore string to previous state. */
      This->Ptr[This->Size] = '\0';
      
      printf("%s %s %d: Size returned by 'vsnprintf(This->Ptr, %zu, Fmt, ap)'"
             " changed.  Was: %d  Is: %d"
             "  This should never happen!\n", __FILE__, __func__, __LINE__,
             BuffSize(This) - This->Size, FullSize, NewFullSize);
      ErrNo = ENOMEM;
      EXIT(EOF);
    } /* Error.  This should never happen! */
  } /* Ran out off space. */
  This->Size = FullSize;
  RETURN(FullSize);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* StringStreamf() */
