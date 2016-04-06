
[comment]: # (-*- mode: markdown; -*-)

# The 'libUtil' utility library

A C library of miscellaneous utilities.  That is, stuff. :)

Author: Leland Best <lcbpublic@gmail.com>

License: GNU GPL v3 or later.  See the file 'COPYING.txt' in this directory.

## <a name="intro"></a>Introduction

This is a library of ... well ... stuff.  It's all stuff I find myself
rewriting, one way or another, whenever I sit down to write a C
program.  (Ultimately, there will be a C++ version but, one thing at a
time.)  It is licensed under the GPL (see the file 'COPYING.txt' in
this directory).  Currently this 'README.md' (plus comments in the
code and, of course, the code itself) is all the documentation there
is.

Since I'm writing this solely with my own use in mind it probably only
builds "out of the box" on GNU/Linux systems with reasonably recent
versions of 'gcc'.  Personally, I only build and test on Debian
'stable' and 'testing' whatever those happen to be at the time
(currently 'jessie' and 'stretch').  I have neither the time, the
resources, nor the motivation (I _did_ mention I'm writing this for my
_own_ use didn't I? :^> ) to worry about anything else.  However,
should somebody be sufficiently motivated, I would _guess_ that it
wouldn't be hard to port to other GNU/Linux systems/distributions.
Maybe even other *nix systems and/or compilers.  Windows?  _Maybe_
Cygwin but who knows?  Beyond that, you are _so_ on your own!

Thus far (which isn't very far at all) I'm planning to have four main areas of
functionality:

1. String conversions (mostly to numeric types).

2. A dynamic string "class" (`struct String` aka `String_t`).

3. Error handling.

4. Command-line argument parsing.

At this point, only the first is really code complete and has had any
testing at all.  The second has the basics for `String_t` but it needs
a _lot_ more functionality and testing.  And I'm not even certain I'll
keep the present interface.  Number three currently consists of a
couple macros and a lot of ideas but that's it.  Number 4 consists
entirely of thoughts about the shortcomings of existing
libraries/packages and ideas about how to fix them, but that's it.

The rest of this document conists of the more-or-less obligatory
"For the Impatient" section followed by a brief discussion of the
build system.  The remainder is as much for myself as for any
foolhardy soul who _really_ wants to know the (possibly convoluted)
logic behind the code or, for the parts that don't exist yet, my
thoughts and ideas about where I might go with them and how I might
get there.  So, without further ado, ...

## For The Impatient

Assuming you have to build this from source, you _should_ be able to
do either the now almost iconic

    prompt> ./configure [options]
    prompt> make
    prompt> make install

or if you prefer `cmake`

    prompt> cd ./build
    prompt> cmake ..
    prompt> make
    prompt> make install

However, amongst other things, you should be aware that at present the
`cmake` approach will _not_ give you a static library (if that's
important to you).  I imagine this is doable in `cmake`.  I just
haven't taken the time to figure out how.  Either way, if you're
lucky, you should then have a library named `libUtil` (sorry for the
incredibly imaginative naming :grimacing: ) that gives you C functions
implementing two-and-a-fraction of the four areas outlined in the
[Introduction](#intro).

### String Conversions

The string conversions provide the equivalent of `strtol()` and
friends for the additional types:

    char, signed char, unsigned char, short, unsigned short, int,
    unsigned int, int8_t, uint8_t, int16_t, uint16_t, int32_t,
    uint32_t, int64_t, uint64_t, int_fast8_t, uint_fast8_t,
    int_fast16_t, uint_fast16_t, int_fast32_t, uint_fast32_t,
    int_fast64_t, uint_fast64_t, int_least8_t, uint_least8_t,
    int_least16_t, uint_least16_t, int_least32_t, uint_least32_t,
    int_least64_t, uint_least64_t

The corresponding functions are:

    strtoc(), strtochar(), strtosc(), strtoschar(), strtouc(),
    strtouchar(), strtos(), strtous(), strtoi(), strtoui(), strtoi8(),
    strtoui8(), strtoi16(), strtoui16(), strtoi32(), strtoui32(),
    strtoi64(), strtoui64(), strtoif8(), strtouif8(), strtoif16(),
    strtouif16(), strtoif32(), strtouif32(), strtoif64(),
    strtouif64(), strtoif8(), strtouil8(), strtoil16(), strtouil16(),
    strtoil32(), strtouil32(), strtoil64(), strtouil64()

All of these except `strtochar()`, `strtoschar()`, and `strtouchar()`
are based on one of the standard C library functions `strtol()`,
`strtoul()`, `strtoll()`, or `strtoull()` and friends and have exactly
analogous semantics for the corresponding types.

The three functions `strtochar()`, `strtoschar()`, and `strtouchar()`
were written from scratch and serve a different purpose.  They convert
a string to a `char`, `signed char`, or `unsigned char` respectively.
The point here being that aside from the obvious conversions (e.g. `"a"`
gets converted to `'a'`) they also handle escape sequences like
`"\n"`, `"\x0d"`, etc..

The `strtoX()` functions are all well and good, however IMO their
error semantics are a bit ... ummmm ... unintuitive.  For example,
`strtoul()` will happily (and silently) convert `"-1"` to `ULONG_MAX`!
So, I wrote a set of functions that have more reasonable (to me) error
handling.  They are:

    StrToChar(), StrToEscChar(), StrToSChar(), StrToEscSChar(),
    StrToUChar(), StrToEscUChar(), StrToShort(), StrToUShort(), ...

and so on.

### Dynamic Strings

This consists of the string "class" `struct String` (aka `String_t`,
and again my apologies for the absolutely briliant naming
:expressionless: ).  It implements a dynamically sized C string.  It
_is_ a C _string_ in that the contents are always `NUL` terminated.
It implements the so called short string optimization with the size of
the "static" buffer for short strings set at compile-time by
`STRING_MIN_BUFF_SIZE` (which defaults to 16).  The automatic buffer
growth faactor is 2.  The buffer never automatically shrinks although
the user can call `StringShrinkToFit()` to force the buffer to shrink
to the size of the current C string (except that it will, obviously,
never be smaller than `STRING_MIN_BUFF_SIZE`).

This being C, not C++, each `String_t` must be initialized by
explicitly calling one of the "constructor" functions (e.g
`ConstructString()`) before it can be used, and must be "destroyed" by
calling `DestroyString()` before it goes out of scope (or the program
exits, or whatever).  That is, something like this:

```C
  void foo() {
    String_t Str;
    ...
    if (!ConstructStringCStr(&Str, "A string.")) {
      /* Error. */
    }

    /* Use 'Str'. */
    ...
    DestroyString(&Str);
  }
```

For `String_t` objects created on the heap one of the `NewString()`
functions can be called to allocate the `String_t` and initialize it
in "one swell foop".  To free such an object call `DeleteString()`.
That is

```C
  void foo() {
    String_t *Str;
    ...
    if ((Str = NewStringCStr(&Str, "A string.")) == NULL) {
      /* Error. */
    }

    /* Use 'Str'. */
    ...
    DeleteString(Str);
  }
```

Currently, beyond the "constructors" and "destructors" there is very
little additonal functionalty.  Probaby the only functions of any real
note are `StringPrintf()` (like `sprintf()` except it will resize the
string to contain the output if necessary), and `StringStreamf()`.
The difference between the two is that while `StringPrintf()` replaces
the contents of `Str` each time, `StringStreamf()` appends the new
output to the existing contents thus allowing a string to be built up
incrementally over several calls.  The idea is sort-of like
`std::ostringstream` in C++ except it has none of the other file-like
functionality (i.e. `seek()`, etc.).  The contents can always be
cleared by calling `StringClear()` in order to start over.

### Error Handling

There is currently so little here, and it's all completely subject to
change, that it's _almost_ not worth mentioning.  However, I'll
briefly demontrate the intended usage of the two main macros,
`RETURN()` and `EXIT()` with this example from `String.c`:

```C
String_t *NewStringStr(const String_t *Src)
{ /* NewStringStr() */
  int ErrNo;
  String_t *This = NULL, *_RetVal;

  /* Error checking. */
  if (Src == NULL)
  { /* Error. */
    printf("%s %s %d: 'Src' is NULL.\n", __FILE__, __func__, __LINE__);
    ErrNo = EFAULT;
    EXIT(NULL);
  } /* Error. */
  
  if ((This = malloc(sizeof(String_t))) == NULL)
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: malloc(%zu) failed.  %s.\n",
           __FILE__, __func__, __LINE__, sizeof(String_t),
           strerror(ErrNo));
    EXIT(NULL);
  } /* Error. */

  if (!ConstructStringStr(This, Src))
  { /* Error. */
    ErrNo = errno;
    printf("%s %s %d: ConstuctStringStr() failed.  %s.\n",
           __FILE__, __func__, __LINE__, strerror(ErrNo));
    EXIT(NULL);
  } /* Error. */
  RETURN(This);

Exit:
  free(This);
  errno = ErrNo;

Return:
  return _RetVal;
} /* NewStringStr() */
```

As an indication of how immature this whole thing is, I'll point out
that this is completely useless if your coding style uses lot of
nested blocks that each create objects (or whatever) that need to be
cleaned up on exit from the block.  That is:

```C
Object2_t *Foo() {
  Object0_t *Obj0;

  if ((Obj0 = malloc(sizeof(Object0_t))) != NULL)
  { /* Obj0 allocated okay. */
    if (InitObject0(Obj0))
    { /* Obj0 initialized okay. */
      Object1_t *Obj1;

      if ((Obj1 = malloc(sizeof(Object1_t))) != NULL)
      { /* Obj1 allocated okay. */
        if (InitObject1(Obj1))
        { /* Obj1 initialized okay. */
          Object2_t *Obj2;
        
          if ((Obj2 = Bar(Obj0, Obj1)) != NULL)
          { /* Bar() okay. */
            ...
            return Obj2;
          } /* Bar() okay. */
          else
          { /* Bar() failed. */
            /* Handle error. */
            ...
            
            FreeObject1(Obj1);
            FreeObject0(Obj0);
            return NULL;
          } /* Bar() failed. */
        } /* Obj1 initialized okay. */
        else
        { /* Error initializing Obj1. */
          /* Handle error. */
          ...

          free(Obj1);
          FreeObject0(Obj0);
          return NULL;
        } /* Error initializing Obj1. */
      } /* Obj1 allocated okay. */
    } /* Obj0 initialized okay. */
    else
    { /* Error initializing Obj0. */
      /* Handle error. */
      ...

      free(Obj0);
      return NULL;
    } /* Error initializing Obj0. */
  } /* Obj0 allocated okay. */
  else
  { /* Error allocating Obj0 */
    /* Handle error. */
    ...
  } /* Error allocating Obj0 */
    
  return NULL;
}
```

I don't see `RETURN()` and/or `EXIT()` being any use here.  In an
ideal world, I'd like to come up with some error handling "stuff" that
would be useful no matter what one's coding style.  However, since I'm
primarily writing `libUtil` for myself and I don't code this way, I'm
not going to loose a lot of sleep if I can't make it work for other
coding styles.

### Command Line Argumets



## Building

At this point I almost entirely use 'cmake' to build 'libUtil' on a
daily basis.  'cmake' is simple to set up and makes reasonable guesses
about the build environment.  It does have what I consider to be one
"too badness" in everyday use: It has nothing like 'autotool's
'distclean' and 'maintainer-clean' targets.  If you do an "in-tree"
build you'll have to clean up all the files and generated by 'cmake'
on your own.  The solution is to do an "out-of-tree" build.  This is
what the empty './build' directory is for.  So, if you want to use
'cmake' I'd suggest doing the following to build and install:

    prompt$ cd <top-level libUtil directory>
    prompt$ cd ./build
    prompt$ cmake ..
    prompt$ make
    prompt$ install    # Optional.

By default, I've set up 'CMakeLists.txt' to install in sub-directories
of '$HOME' but you can change this by editing this line:

    # Set up the install stuff.
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}")

Note that unless you've changed something in 'CMakeLists.txt' or
'Util/CMakeLists.txt', you only need to run `cmake ..` once.

Once you've built in './build' you can do the equivalent of `make
maintainer-clean` by doing this:

    prompt$ cd <top-level libUtil directory>
    prompt$ cd ./build
    prompt$ make uninstall    # Optional.
    prompt$ make clean
    prompt$ rm -rf *

WARNING: Be _sure_ you are in './build' when you do the last step!

So.  Why keep all the extra stuff for GNU 'autotools' around too?  I
first tarted using Free Software back in the 1990's while working on
my PhD.  Even before Linux was on the scene I was downloading GNU
software to the SGI workstations I was using because they offered more
functionality and, generally, seemed more reliable than the native SGI
tools.  In particular, from my point of view, 'gcc' won hands down
over the native 'cc' on the strength of it's error messages alone!
And GNU 'make' ... well ... there was no comparison.  But the most
astounding thing was that, with the exception of 'gcc' I could just
download the tarball, unpack it, type './configure', 'make', and 'make
install' ... and it worked!  Magic!

Well, I now know that the system behind the magic was 'autotools.  I
also know that this takes a lot of work by the developer(s).  But in
my dreams, if this library were ever to become popular, I'd like users
to have that same "magic" experience.  Hence the 'autotools'
framework.  So, in principle you should also be able to do:

    prompt$ cd <top-level libUtil directory>
    prompt$ ./configure [options]
    prompt$ make
    prompt$ make install

to build and install 'libUtil'.  (Also, see 'INSTALL.txt' for more
info.)  Unfortunately, as noted earlier, I have neither the time nor
the resources to do the necessary work to make sure this works on
anything other than recent Debian GNU/Linux systems.  Once I get all
the pieces of 'libUtil' in place and working for me I'll welcome help
in this area.

## String Conversions

The standard C library, at least as of C99, provides `atoi()`,
`atol()`, `atoll()`, `atof()`, `strtol()`, `strtoll()`, `strtoul()`, ,
`strtoull()`, `strtof()`, `strtod()`, and `strtold()`.  The `atoX()`
family of functions provide _no_ error detection.  None.  Period.  As
such, I consider them useless.  The `strtoX()` functions do provide
error detection.  In addition, they cover a wider range of numeric
types and, for integers, conversion from a limited number of bases
(base 2 - base 36).  Nevertheless, these days there are quite a number
of numeric types not directly covered by these functions.  These
include:

    char, signed char, unsigned char, short, unsigned short, int,
    unsigned int, int8_t, uint8_t, int16_t, uint16_t, int32_t,
    uint32_t, int64_t, uint64_t, int_fast8_t, uint_fast8_t,
    int_fast16_t, uint_fast16_t, int_fast32_t, uint_fast32_t,
    int_fast64_t, uint_fast64_t, int_least8_t, uint_least8_t,
    int_least16_t, uint_least16_t, int_least32_t, uint_least32_t,
    int_least64_t, uint_least64_t

Ignoring, for the moment, the variations on `char`, if `long long` is
at least as big as all flavors of `int64`, clearly conversions to all
these types can be accomplished using the functions in the standard C
library plus some extra code.  So, rather than cut-and-pate (or write
it from scratch if I can't find it) every time I need to do one of
these I've finally just put them all in 'libUtil'.  The corresponding
functions are:

    strtos(), strtous(), strtoi(), strtoui(), strtoi8(), strtoui8(),
    strtoi16(), strtoui16(), strtoi32(), strtoui32(), strtoi64(),
    strtoui64(), strtoif8(), strtouif8(), strtoif16(), strtouif16(),
    strtoif32(), strtouif32(), strtoif64(), strtouif64(), strtoif8(),
    strtouil8(), strtoil16(), strtouil16(), strtoil32(), strtouil32(),
    strtoil64(), strtouil64()

All of these take arguments exactly analogous to `strtol()`.  That is,
`strtos()` is really:

    short strtos(const char *Str, char **End, int Base);

and so on.  Error handling semantics, etc. are also exactly
analogous.

[comment]: # (We should "tweak" the .h files, etc. so that, where)
[comment]: # (possible, they reduce to 'inline' functions.)

In fact, for some types, if the `MAX` and `MIN` values are
the same, macros simply reduce the function to a call to the
appropriate `strto(u)l(l)()` function.

String to `char` conversion (and relatives) are a special case.  There
are two reasonable ways to convert a string to a character.  The first
is to simply convert it as a `char` sized integer.  That is, a valid
`signed char` might accept strings like "0x0d", "10", or "-128"
(assuming a `signed char` is an 8-bit two's complement number).  The
second is to accept C-style strings representing a single character.
For example: "a", "0", "=", "\n", "\t", "\xff", etc..

[comment]: # (These function names are not correct, and the former)
[comment]: # (three are nonexistent.  We need to fix this.)

Both of these are supported by six functions.  The straight numeric
conversions are:

    strtoc(), strtosc(), strtouc()

while the single character string conversions are:

    strtochar(), strtoschar(), strtouchar()

Again, return values and error semantics are as close to `strtol()` etc
al as makes sense.

So much for the easy stuff.  While the `strtoX()` functions do report
errors, it is still a bit obscure.  The `StrToX()` family of functions
take care of all this error checking and return a valid result _only_
if the input string consists _entirely_ of a value representable as
the appropriate type.  For example, from `StrToLong.c`:

    int StrToLong(const char *Str, int Base, long *Val)

> Equivalent to `strtol()` but with simplified error reporting.
> Returns the `errno` value rather than setting the global `errno`.
> The global `errno` is always left unchanged.  It is an error not to
> use all of `Str`, or for `(*Str == '\0')`.  Returns `ERANGE` on
> overflow/underflow, `EINVAL` for a malformed `Str`, `EDOM` for an
> invalid `Base`, and `EFAULT` if either `Str` or `Val` is `NULL`.  If
> the function returns non-zero then `*Val` is unchanged.

All other `StrToX()` functions are exactly analogous (except for those
returning floating point types and the `StrToEscChar()` functions
none of which have a `Base` argument).

### Coding StrToX() Functions

Mostly for my own future edification (and for anyone else with
sufficient curiosity, boredom, whatever) we'll now examine just how
much work/logic is involved in producing these results.  We'll examine
three functions which each add additional complexity to the problem.
We'll start with `StrToLong()`, progress to `StrToULong()`, and finish
with `StrToUShort()`.  The extensions to other types should be
"obvious to the most casual observer" :smiling_imp: .  So, here we go!

#### StrToLong()

According to "man strtol" on my Debian GNU/Linux systems (both
"jessie" and "stretch"):

> long int strtol(const char *nptr, char **endptr, int base);
> ...
> The strtol() function converts the initial part of the string in nptr
> to a long integer value according to the given base, which must be
> between 2 and 36 inclusive, or be the special value 0.  ...
> 
> The string may begin with an arbitrary amount of white space (as
> determined by isspace(3)) followed by a single optional '+' or '-'
> sign.  ...
>
> The remainder of the string is converted to a long int value in the
> obvious manner, stopping at the first character which is not a valid
> digit in the given base.

Similarly, [The Single Unix Specification][1] says:

> long strtol(const char *restrict str, char **restrict endptr, int base);
>
> ...
> 
> These functions shall convert the initial portion of the string pointed to by str to a type long and long long representation, respectively. First, they decompose the input string into three parts:
> 
>   1. An initial, possibly empty, sequence of white-space characters
>      (as specified by isspace())
> 
>   2. A subject sequence interpreted as an integer represented in
>      some radix determined by the value of base
> 
>   3. A final string of one or more unrecognized characters,
>      including the terminating NUL character of the input string.
> 
> Then they shall attempt to convert the subject sequence to an
> integer, and return the result.

It seems to me that these should raise some eyebrows right off the
bat.  While I understand that skipping white-space and stopping at the
first non-"digit" character _might_ be useful in very simple
parser-like applications, it also means that strings like "123 4th
Street", "1st Item:", and " 80 40 " will all be accepted as valid
strings.  And, in fact, they are.  To be fair, `strtol()` _does_
return _almost_ enough information (via `errno` and the `endptr`
argument) to tell whether it's argument consists _only_ of a single
integer representable as a `long` but it's far from obvious how to
extract/guarantee this result.

First of all, like most functions in the standard C library,
`strtol()` says nothing about what happens if `str` (`nptr` in the man
page) is `NULL`.  This means that it's behavior in this case is
_implicitly_ undefined.  To be clear, this means the result could be
_anything!_  It might return zero, cause a segfault, quietly
corrupt the heap, etc..  Therefore, the first lines of code in
`StrToLong()` check for these kinds of cases and handle them
"gracefully":

```C
int StrToLong(const char *Str, int Base, long *Val)
{ /* StrToLong(const char *, int, long *) */
  char *End = NULL;
  long NewVal;
  int SavedErrNo, ErrNo;

  /* NULL pointer checking. */
  if (Str == NULL || Val == NULL)
  { /* Error. */
    return EFAULT;
  } /* Error. */
```
  
We return `EFAULT` because according to `man errno` we have

> EFAULT          Bad address (POSIX.1)

which sounds appropriate.  Now that we know that `(Str != NULL)`, we
can deal with the possibility that `Str` begins with "An initial,
possibly [non-]empty, sequence of white-space characters (as specified by
isspace())".  Several "NOTE:"s are in order here. :smiling_imp:
NOTES:

1. If you read the Debian GNU/Linux man page you'll find this
   statement: "If endptr is not `NULL`, `strtol()` stores the address of
   the first invalid character in `*endptr`.  If there were no digits at
   all, `strtol()` stores the original value of `nptr` in `*endptr` (and
   returns 0)."

   This seems a bit self contradictory.  Suppose `str` is " foo" (and
   assume `base` is 10).  On the one hand, the first invalid character
   if the `'f'` (remember, `strtol()` considers leading white-space as
   valid) so `*endptr'` should be `'f'`.  On the other hand, there are
   "no digits at all" so `(endptr == str)` seems appropriate.  And
   while I'm at it, what if `str` is " a1"?  The first invalid
   character is `'a'` and there _is_ a digit, so, taking the above
   description literally, `*endptr` _must_ be `'a'`.  But ... really?
   In point of fact, no.  On Debian GNU/Linux `strtol()` returns 0 and sets
   `(endptr == str)`.

   [The Single Unix Specification][1] is clearer on the subject (though
   a bit long winded):

   "The subject sequence is defined as the longest initial subsequence
   of the input string, starting with the first non-white-space
   character that is of the expected form. The subject sequence shall
   contain no characters if the input string is empty or consists
   entirely of white-space characters, or if the first non-white-space
   character is other than a sign or a permissible letter or digit.
   ...

   If the subject sequence is empty or does not have the expected
   form, no conversion is performed; the value of `str` shall be stored
   in the object pointed to by `endptr`, provided that `endptr` is not a
   null pointer.".

2. If you read the Debian GNU/Linux man page you'll also find this
   statement: "... if `*nptr` is not `'\0'` but `**endptr` is `'\0'`
   on return, the entire string is valid.".  In our case this is
   somewhat misleading as their definition of a "valid" string may
   include leading white-space.

3. Speaking of white-space, the man page for `isspace()` says
   "... checks for white-space characters.  In the "C" and "POSIX"
   locales, these are: space, form-feed (`'\f'`), newline (`'\n'`),
   carriage return (`'\r'`), horizontal tab (`'\t'`), and vertical tab
   (`'\v'`).".  [The Single Unix Specification][1] is even less
   helpful: "The isspace() ... and isspace_l() ... functions shall
   test whether c is a character of class space in the current locale,
   ...  or in the locale represented by locale, ... respectively;".
   Now I admit to being a bit pedantic here, but not knowing any
   better I have to ask whether _somewhere_ there might not be a
   locale where `'\0'` is considered white-space?

With all this ambiguity floating around I should just take matters
into my own hands and check for the empty string or leading white-space
explicitly.  So, given that we now know `Str` is not `NULL`, the next
chunk of code is:

```C
  /* Check that 'Str' is not empty, nor does it start with white space
   * both of which strtol() considers valid. */
  if (*Str == '\0' || isspace(*Str))
  { /* Error. */
    return EINVAL;
  } /* Error. */
```

Next up: Dealing with the `Base` argument.  The man page doesn't
address the behavior of `strtol()` if passed an invalid `base` other
than to say that it "... must be between 2 and 36 inclusive, or be the
special value 0." and, under "ERRORS", "EINVAL (not in C99) The given
base contains an unsupported value.".
[The Single Unix Specification][1] is basically the same saying, also
under "ERRORS", "[CX] [Option Start] If the value of base is not
supported, 0 shall be returned and errno shall be set to
[EINVAL]. [Option End]" where the "[CX] [Option Start]
... [Option End]" pair translates into "This is a POSIX.1-2008
extension.".  So, basically, we need to check the value of `Base`
ourselves:

```C
  /* We don't rely on strtol() to check for an invalid base because,
   * depending on standards conformance, in this case it's behavior
   * may be undefined. */
  if (Base != 0 && (Base < 2 || Base > 36))
  { /* Invalid base. */
    return EDOM;
  } /* Invalid base. */
```

We choose to return `EDOM` because `man errno` says "EDOM Mathematics
argument out of domain of function (POSIX.1, C99)" where, according to
[Wikipedia](https://en.wikipedia.org/wiki/Domain_of_a_function
Wikipedia definition of domain) the "domain" of a function is "... the
domain of definition (or simply the domain) of a function is the set
of "input" or argument values for which the function is defined.".

Okay, at this point we know that `Str` is not `NULL`, `Val` is not
`NULL`, `Str` is not empty nor does it start with white-space, and
`Base` is valid.  Only a couple more details before we can actually
call `strtol()`.

We want to guarantee that we don't change `errno`.  Since `strtol()`
itself can set `errno` we must save the current value so we can
restore it later, then set it to 0 to detect errors returned from
`strtol()`.  (Since `strtol()` returns a `long`, on most systems it is
logically impossible for it to return an invalid value to indicate an
error.  That is, there is no `long` equivalent to, say, `NaN` or
`Inf`, for integer types.)  Then finally, we can call `strtol()` an
save `errno` for later use:

```C
  /* strtol() returns error status in 'errno' but we must leave it
   * unchanged. */
  SavedErrNo = errno;

  /* Do the conversion. */
  errno = 0;
  NewVal = strtol(Str, &End, Base);
  ErrNo = errno;
```

WARNING: GNU `libc` allocates `errno` in "thread local storage" (TLS)
so on any such system this is sufficient to guarantee that we can
leave `errno` unchanged even in a multithreaded environment.  If we
are running under a `libc` that doesn't do this then the caller is
responsible for doing any necessary locking (or whatever) to ensure
that no other threads change `errno` on us.  Anyway, ...

Whew!  We're almost there!  To recap, we now know neither `Str` nor
`Val` is `NULL`, `Base` is valid, and `Str` is neither empty nor does
it start with white-space.  Therefore, `Str` must be at least one
non-whitecap's character followed by zero or more characters.  This in
turn implies that `Str` is composed of a (possibly empty) set of
characters we'll call "N" that represent an integer in base `Base`,
followed by a (possibly empty) set of other characters which we'll
call "S".  Also note that:

1. If "S" is empty, then "N" is non-empty since we know `Str` is
   non-empty.

2. If "N" is empty then "S" is non-empty _and_ does not start with
   white-space.

In short we are left with only two possible error conditions:

1. `Str` is malformed.  Equivalently, this now implies that "S" is
   non-empty and does not start with white-space if "N" is empty.

2. `Str` is out of range.  That is, "S" is empty but "N" cannot be
   represented by a `long` because "S" represents a value that is
   either too large (positive overflow) or too small (negative
   underflow).

According to both the man page and [The Single Unix Specification][1]
if `Str` is entirely valid (i.e. "S" is empty) _except_ that it is out
of range, then `(*endptr == '\0')`.  Specifically, from the man page
we have:

> The remainder of the string is converted to a long int value in the
> obvious manner, stopping at the first character which is not a valid
> digit in the given base.

But if "S" is empty then "the first character which is not a valid
digit in the given base" must be the terminating `'\0'`.  From
[The Single Unix Specification][1] we have:

> The subject sequence is defined as the longest initial subsequence of
> the input string, starting with the first non-whitespace character
> that is of the expected form.

That is, `strtol()` is guaranteed to consume all of "N" which, again,
leaves `EndPtr` pointing at the terminating `'\0'`.  So, we can check
`*EndPtr` to see whether or not "S" was empty.  If not we return an
error because that makes `Str` malformed in the context of
`StrToLong()`.  If "S" was empty (`Str` was well formed) then it only
remains to check the saved value of `errno` to see whether we had
overflow or underflow.  If so, return the saved `errno`, otherwise
restore the saved `errno`, the converted value, and return 0:

```C
  if (*End != '\0')
  { /* 'Str' is invalid. */
    ErrNo = EINVAL;
  } /* 'Str' is invalid. */

  else if (ErrNo == 0)
  { /* No error, so return converted value. */
    *Val = NewVal;
  } /* No error, so return converted value. */

  /* Restore global 'errno' and return 'ErrNo'. */
  errno = SavedErrNo;
  return ErrNo;
} /* StrToLong(const char *, int, long *) */
```

Piece of cake ... not! :frowning:


#### StrToULong()

One would think that, having waded through all the details of writing
`StoToLong()`, `StrToULong()` would simply be a matter of calling
`stroul()` at the appropriate place.  Sadly, no such luck. :frowning:
While much of the code is the same, there is one added complication.
From the Debian GNU/Linux man page for `strtoul()' we have:

> The string may begin with an arbitrary amount of white space (as
> determined by isspace(3)) followed by a single optional '+' or '-'
> sign.

No, that's not a copy-paste error. `strtoul()` (and `strtoull()` while
we're at it) will silently convert negative values to positive ones
... and not flag it as an error at all! :astonished:
[The Single Unix Specification][1] confirms this:

    unsigned long strtoul(const char *restrict str,
        char **restrict endptr, int base);

...

> ... the expected form of the subject sequence is that of a decimal constant, octal constant, or hexadecimal constant, any of which may be preceded by a '+' or '-' sign.

How do we handle this?  Well, we can keep all the code that has
nothing to do with the type we're trying to convert to:

```C
int StrToULong(const char *Str, int Base, unsigned long *Val)
{ /* StrToULong(const char *, int, unsigned long *) */
  char *End = NULL;
  unsigned long NewVal;
  int SavedErrNo, ErrNo;

  /* NULL pointer checking. */
  if (Str == NULL || Val == NULL)
  { /* Error. */
    return EFAULT;
  } /* Error. */

  /* Check that 'Str' is not empty, nor does it start with white space
   * both of which strtoul() considers valid. */
  if (*Str == '\0' || isspace(*Str))
  { /* Error. */
    return EINVAL;
  } /* Error. */

  /* We don't rely on strtoul() to check for an invalid base because,
   * depending on standards conformance, in this case it's behavior
   * may be undefined. */
  if (Base != 0 && (Base < 2 || Base > 36))
  { /* Invalid base. */
    return EDOM;
  } /* Invalid base. */
```

Now in `StrToLong()` this would be the point where we would get ready
to call `strtol()`.  Fortunately, this turns out to be a good place to
check whether 'Str' represents a negative number.  At this point we
already know that `Str` is not `NULL`, is not empty, and does not
start with white-space.  If `Str` at least starts with a well formed
integer then the first character _must_ be either a `'+'`, a `'-'`, or
a valid digit in the selected base.  For a negative value it must be
`'-'`.  So, all we have to do is:

```C
  if (*Str == '-')
  { /* Negative number. */
    return ERANGE;
  } /* Negative number. */
```

We return `ERANGE` because negative numbers are (or _should_ be
:angry: ) out of the range that can be represented by an unsigned
type.

The remainder of the code is identical to `StrToLong()` except that we
call `strtoul()` instead of `strtol()'.

#### StrToShort()

Now we really are almost done.  All our work writing the myriad of
`strtoX()` functions pays off.  `StrToShort()` is identical to
`StrToLong()` except that we call (our own) `strtos()` instead of
'strtol()'.

### String Conversions, Conclusion

It's been amazing how hard it has been to get this seemingly simple
functionality right.  In fact, looking back on things, I doubt that
I've ever got it right before.  But then, that's the whole point of
`libUtil`.  Take a bunch of stuff that _should_ be easy but isn't,
wrap it up in code that _does_ make it easy, and stick it in a
library.  For string conversions that's now done ... and I hope never
to have to do it again.

## Dynamic C Strings

## Error Handling
Some notes on error handling.

First, our objective: We want to provide a reusable "framework" for
handling (run-time) errors.  That is, we want to define functions,
structs, macros, <whatever> that, ideally, make writing error
handling code less repetitive and more reliable.  However, before
we can even begin this task we need to define what we mean by an
"error".

It is tempting to offer up some platitude such as this excerpt from
http://en.cppreference.com/w/cpp/language/exceptions:

"... 'errors' are typically limited to only the following:

1. Failures to meet the postconditions, such as failing to
produce a valid return value object

2. Failures to meet the preconditions of another function that
must be called (for non-private member functions)

3. Failures to (re)establish a class invariant"

Sounds good, right?  Hmmm.

Okay, let's consider 1 above.  Certainly one of the most obvious
ways for this to happen is if 'malloc()' fails.  Ask almost any
"garden variety" programmer whether failure of 'malloc()'
(i.e. 'malloc()' returns 'NULL') is an error and they'll
categorically say "Yes!".

In one way this is correct.  That is, when a programmer calls
'malloc()' he/she almost always _expects_ to receive a pointer to a
chunk of memory.  Furthermore, if 'malloc()' fails the programmer
may find it difficult to decide what to do about it.  In this
sense, failure of 'malloc()' is almost always an error.

On the other hand, there are certainly many cases when failure of
'malloc()' should not (or even _must_ not) cause program
termination (which, BTW, is the default behavior in C++).  Consider
an interactive image processing/manipulation program (i.e. The
Gimp).  Now suppose a naive user asks it to create a
2,000,000x2,000,000 image (that's 4x10^12 pixels).  On almost any
modern day (2016) computer, attempting to 'malloc()' that much
space will fail.  But with a GUI interface this can, at some level,
be handled by asking the user to choose a smaller image size.  In
this sense, failure of 'malloc()' is no more an error than any of
thousands (millions?) of other possible conditions.  Then again, if
the same program can also be run non-interactively (i.e. in
"batch" mode) then, since we cannot ask the user for a different
image size, the exact same error may be fatal.

Aside from recoverability, there can also be other significant
differences between these two cases.  In the interactive case there
is no need to output detailed information about where and why the
error occurred (i.e. debug info).  In fact, such output would
probably just confuse the user.  On the other hand, in batch mode
it may well be useful to output such info so the writer of the
script (or whatever) can track down what went wrong.  But wait!
Suppose the program is called from a script that is "smart" enough
to be able to try a smaller image size?  Now we're back to a state
where the debug info will just be a nuisance!  So, at the very
least, what constitutes an "error" is application (program)
dependent, and possibly even context dependent.

The problem of determining what constitutes an error and/or what to
do about it becomes even more unclear in the case where we are
developing a library.  In the case of an application program we can
hope that in situations like the ones outlined above some form of
"user" input may tell us how to handle such "errors".  In a library
we have no clue what our application and/or context may be.
Indeed, one sign of a "good" library is that it winds up being used
(successfully :) ) in ways the original author never imagined.

The bottom line here seems to be two-fold:

1. We can not hope to write an error handling library/package that
determines what is an error or how to handle one when it occurs.

2. We _can_ hope to more-or-less encapsulate tasks frequently
required when handling errors (e.g.





If the function completes successfully two things must happen:

1) Any local/temporary resources must be released (e.g. files may
need to be closed, memory free()'ed, mutexes unlocked, etc.).
 *
2) The function must return it's intended value.
 *
On the other hand, in the event of a error, three things must happen:
 *
1) The error must be reported.  This may take many forms from simply setting/returning an error code, to logging/recording detailed debugging info about what happened, to trying to continuing
 *
Searching Google for some kind of consensus reveals, basically, that
there is none. :( At least, nothing beyond being consistent within
a library.

A number of posts described/provided macro packages
that, at their core, use 'setjmp()'/'longjmp()' to try to mimic C++
"exceptions".  While I admit that I think C++ "exceptions" are evil
to begin with (I know: Heresy!) the 'setjmp()'/'longjmp()' macros
seem particularly nasty since C has no concept of "real" "stack
unwinding" inasmuch as there are no destructors to automatically
clean up objects allocated on the stack.  Therefore, one small
slip-up can lead to a whole slew of orphaned/leaked stuff floating
around.  As a result, I hereby reject any such solutions out-of-hand.

## Command-line Arguments


[1]:(http://pubs.opengroup.org/onlinepubs/9699919799/toc.htm)
