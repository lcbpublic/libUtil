
[comment]: # (-*- mode: markdown; -*-)

# The 'libUtil' utility library

A C library of miscellaneous utilities.  That is, stuff. :)

Author: Leland Best <lcbpublic@gmail.com>

License: GNU GPL v3 or later.  See the file 'COPYING' in this directory.


## <a name="intro"></a>Introduction

This is a library of ... well ... stuff.  It's all stuff I find myself
rewriting, one way or another, whenever I sit down to write a C
program.  (Ultimately, there will be a C++ version but, one thing at a
time.)  It is licensed under the GPL (see the file 'COPYING' in
this directory).  Currently this 'README.md' (plus comments in the
code and, of course, the code itself) is all the documentation there
is.

Since, for the time being, I'm writing this solely with my own use in
mind it probably only builds "out of the box" on GNU/Linux systems
with reasonably recent versions of 'gcc'.  Personally, I only build
and test on Debian 'stable' and 'testing' whatever those happen to be
at the time (currently 'jessie' and 'stretch').  I have neither the
time, the resources, nor the motivation (I _did_ mention I'm writing
this for my _own_ use didn't I? :^> ) to worry about anything else.
However, should somebody be sufficiently motivated, I would _guess_
that it wouldn't be hard to port to other GNU/Linux
systems/distributions.  Maybe even other *nix systems and/or
compilers.  Windows?  _Maybe_ Cygwin but who knows?  Beyond that, you
are _so_ on your own!  For more details see [Building](#building)
below.

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
libraries/packages and ideas about how to fix them.  Period.

The rest of this document consists of the more-or-less obligatory
[For the Impatient](#impatient) section followed by more detailed
discussion of rationals, decisions, etc. about why the code does what
it does.  This latter stuff is as much for myself as for any foolhardy
soul who _really_ wants to know the (possibly convoluted) logic behind
the code or, for the parts that don't exist yet, my thoughts and ideas
about where I might go with them and how I might get there.  So,
without further ado, ...


## <a name="impatient"></a>For The Impatient

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

    strtochar(), strtoschar(), strtouchar(),

    strtoc(), strtosc(), strtouc(), strtos(), strtous(), strtoi(),
    strtoui()

    strtoi8(), strtoui8(), strtoi16(), strtoui16(), strtoi32(),
    strtoui32(), strtoi64(), strtoui64()

    strtoif8(), strtouif8(), strtoif16(), strtouif16(), strtoif32(),
    strtouif32(), strtoif64(), strtouif64()

    strtoif8(), strtouil8(), strtoil16(), strtouil16(), strtoil32(),
    strtouil32(), strtoil64(), strtouil64()

All of these except `strtochar()`, `strtoschar()`, and `strtouchar()`
are based on one of the standard C library functions `strtol()`,
`strtoul()`, `strtoll()`, or `strtoull()` and friends and have exactly
analogous semantics for the corresponding types.

The three functions `strtochar()`, `strtoschar()`, and `strtouchar()`
were written from scratch and serve a different purpose.  They convert
a string to a `char`, `signed char`, or `unsigned char` respectively.
The point here being that aside from the obvious conversions (e.g. `"a"`
gets converted to `'a'`) they also handle escape sequences like
`"\n"`, `"\x0d"`, etc..  **NOTE**: In case it's not obvious, these
functions do not and, as currently defined, can not handle the C99
`\unnnn` and `\Unnnn` escape since these may return more than one
character.

The `strtoX()` functions are all well and good, however IMO their
error semantics are a bit ... ummmm ... unintuitive.  For example,
`strtoul()` will happily (and silently) convert `"-1"` to
`ULONG_MAX`(!) and `-ULONG_MAX-1` to 1(!!).  So, I wrote a set of
functions that have more reasonable (to me) error handling.  They are:

    StrToChar(), StrToSChar(), StrToUChar(),

    StrToC(), StrToSC(), StrToUC(), StrToS(), StrToUS(), StrToI(),
    StrToUI(), StrToL(), StrToUL(), StrToLL(), StrToULL(),
    StrToIMax(), StrToUMax(), StrToF(), StrToD(), StrToLD(),

    StrToI8(), StrToUI8(), StrToI16(), StrToUI16(), StrToI32(),
    StrToUI32(), StrToI64(), StrToUI64(), StrToIMax(), StrToUMax(),
    
    StrToIF8(), StrToUIF8(), StrToIF16(), StrToUIF16(), StrToIF32(),
    StrToUIF32(), StrToIF64(), StrToUIF64(),

    StrToIL8(), StrToUIL8(), StrToIL16(), StrToUIL16(), StrToIL32(),
    StrToUIL32(), StrToIL64(), StrToUIL64()

and so on.  These functions 0 on success, or a (positive) `errno`
value on error.  The converted value is returned through a pointer to
the appropriate type.  It is an error for any pointer argument to be
`NULL` (`EFAULT`), for `(Base != 0 && (Base < 2 || Base > 36))`
(`EDOM`), for `Str` to not be entirely composed of what
[The Single Unix Specification][1] calls "the subject sequence"
(`EINVAL`), or for `Str` to not be representable in the requested type
(`ERANGE`).


### <a name="intro_string"></a>Dynamic Strings

This consists of the string "class" `struct String` (aka `String_t`,
and again my apologies for the absolutely brilliant naming
:expressionless: ).  It implements a dynamically sized C string.  It
is a C _string_ in that the contents are always `NUL` terminated.
It implements the so called short string optimization with the size of
the "static" buffer for short strings set at compile-time by
`STRING_MIN_BUFF_SIZE` (which defaults to 16).  The automatic buffer
growth factor is 2.  The buffer never automatically shrinks although
the user can call `StringShrinkToFit()` to force the buffer to shrink
to the size of the current C string (except that it will, obviously,
never be smaller than `STRING_MIN_BUFF_SIZE`).

This being C, not C++, each `String_t` _must_ be initialized by
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
little additional functionality.  Probably the only functions of any real
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

There is currently so little here, and it's all so completely subject
to change, that it's _almost_ not worth mentioning.  However, I'll
briefly mention the few bits that do exist.  These are a couple of
macros, and the function `StrError()`.  See
[Error Handling](#error_handling) for more info.


### Command Line Arguments

Currently, there is nothing here whatsoever in terms of code.  The
objective however, is to come up with something like the GNU Argp
library only better.  As a general concept GNU Argp does one big thing
I want: It forces the definition of "options" and the usage/help/error
messages to stay in sync by automatically formatting them based on
info provided by the user/calling application.  However, it still has
several inherent shortcomings as outlined below.  Needless to say, the
`getopt()` family of functions (henceforth, simply `getopt()`), on
which Argp appears to be based, offers even less functionality "out of
the box".


#### Formatting of usage/help/error messages.

In `getopt()` only error messages are generated automatically.  Argp
additionally generates "usage" and "help" messages automatically
which, as noted above, is a huge plus.  However, the formatting of
these messages is fixed (and, at least in Debian "jessie" and
"stretch", are even wrong in some circumstances).  Given the
flexibility of Argp in other ways, I find it surprising that Argp
doesn't provide "hooks" for the application programmer to supply
his/her own formatting function(s).  Nevertheless, that's the way it
is and, as a result, I'd like my library to fix these shortcomings.
Obviously I'd like to supply reasonable default formatting, but I also
want to provided a reasonably intuitive way for the user to override
the defaults.


#### Argument/Option semantics.

Neither `getopt()` nor Argp provide and _semantic_ processing of
arguments and/or options.  For example, converting an option argument
(a C string) to a `double` and storing it in some variable.  Thus, the
user is reduced to having to (re)write the venerable but error-prone

```C
      switch(CharCode) {
        case 'a':
        case LONGOPT_A:
          /* Handle option '-a'/'--opt-a'. */
          break;

        ...
      }

      /* Handle "positional" arguments. */
```

I want to eliminate this.  Obviously, I'm still working out how to
implement this but I think the general idea will be to have the input
to the parser function include _objects_ that know how to parse
various types of things.  These would be associated with other info
defining the command-line syntax (e.g. option name(s), etc.).  In
compiler jargon, the objects would implement the "semantic actions" of
the parser.  Obviously, this library can't provide parser objects for
every conceivable type of input so it should also be relatively
straight forward for the user to define their own parser objects.


#### Naming Flexibility

I'd want greater flexibility in the naming of "options" and the
arguments they can take.

Argp, along with the 'getopt()' family of functions, require that all
options start with either '-' or '--'.  Despite this being a GNU coding
"standard" I find this unnecessarily restrictive.  As just one example,
many X programs (including the X server itself) use `-opt` to turn a
feature off and `+opt` to turn it on.

Both Argp and `getopt()` require that options have exactly zero or one
argument.  I can imagine many applications where an option might
"naturally" be expected to take two or more arguments.  For example,
start and end points for some computation: `integrate --limits 0 10
data_file.txt`.  In all fairness, this _can_ be handled by both Argp
and `getopt()`.  One can use separate start and end options
(`integrate --lower 0 --upper 10 data_file.txt`) or one argument that
contains both pieces of info (`integrate --limits "0 10"
data_file.txt`).  However, IMO the former tends to lead to a less
natural and more verbose command-line while the latter may require
significant effort on the part of the application programmer to parse
the single argument into it's constituent values.  I'd like to expand
this restriction to allow options to take multiple arguments.


### Jumping to Conclusions

Though it turned out much longer than expected, the above is a _brief_
overview of what currently exists in `libUtil` along with a bit about
what I need/want to add.  If you're seriously interested in the code
itself and/or more details about my thinking about what still needs to
be done, read the sections below.  Be warned though, it can get quite
tedious.  Basically, what I'm doing in `libUtil` is extending and
improving ([cough] fixing [cough]) functionality that is either ill
defined or entirely missing in the standard C library, so I go to some
effort to more-or-less prove that the behavior of the new
functionality is always "correct", where "correct" is somewhat
context dependent.


## <a name="building"></a>Building

At this point I almost entirely use `cmake` to build `libUtil` on a
daily basis.  `cmake` is simple to set up and makes reasonable guesses
about the build environment.  It does have what I consider to be one
"too badness" in everyday use: It has nothing like the `autotools`
`distclean` and `maintainer-clean` targets.  Therefore, if you do an
"in-tree" build you'll have to clean up all the files and directories
generated by `cmake` on your own.  The solution (which is _strongly_
encouraged by everything I've read about `cmake`) is to do an
"out-of-tree" build.  This is what the empty './build' directory is
for.  So, if you want to use `cmake` I'd suggest doing the following
to build and install:

    prompt$ cd <top-level libUtil directory>
    prompt$ cd ./build
    prompt$ cmake ..
    prompt$ make
    prompt$ install    # Optional.

By default, I've set up `CMakeLists.txt` to install in sub-directories
of `$HOME` but you can change this by editing this line:

    # Set up the install stuff.
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}")

Note that unless you've changed something in `CMakeLists.txt` or
`Util/CMakeLists.txt`, you only need to run `cmake ..` once.

Once you've built in './build' you can do the equivalent of `make
maintainer-clean` by doing this:

    prompt$ cd <top-level libUtil directory>
    prompt$ cd ./build
    prompt$ make uninstall    # Optional.
    prompt$ make clean
    prompt$ rm -rf *

WARNING: Be _sure_ you are in `./build` when you do the last step!

So.  Why keep all the extra stuff for GNU `autotools` around too?  I
first tarted using Free Software back in the 1990's while working on
my PhD.  Even before Linux was on the scene I was downloading GNU
software to the SGI workstations I was using because they offered more
functionality and, generally, seemed more reliable than the native SGI
tools.  In particular, from my point of view, GNU's `gcc` won hands
down over the native `cc` on the strength of it's error messages
alone!  And GNU `make` ... well ... there was no comparison.  But the
most astounding thing was that, with the exception of `gcc` I could
just download the tarball, unpack it, type `./configure`, `make`, and
`make install` ... and it worked!  Magic!

Well, I now know that the system behind the magic was `autotools`.  I
also know that this takes a lot of work by the developer(s).  In my
dreams, if this library were ever to become popular, I'd like users to
have that same "magic" experience.  Hence the `autotools` framework.
So, in principle you should also be able to do:

    prompt$ cd <top-level libUtil directory>
    prompt$ ./configure [options]
    prompt$ make
    prompt$ make install

to build and install `libUtil`.  (Also, see `INSTALL` for more info.)
Unfortunately, as noted earlier, this does require considerable
additional effort on the part of the developer/maintainer.  In
principle, and if I'm understanding things correctly, the code needs to
test all the appropriate `HAVE_xxx` macros in `config.h` and do "the
right thing" based on the result.  So far, I have no such tests/code.
And even if I did, at present I'd have neither the time nor the
resources to make sure this I was doing it correctly so that it would
work on anything other than recent Debian GNU/Linux systems.  Oh.  And
then there's "internationalization".  I don't even know how to do this
_in principle_!  Once I get all the pieces of `libUtil` in place and
working for me, I'll welcome/need help in these areas.


## String Conversions

The standard C library, at least as of C99, provides `atoi()`,
`atol()`, `atoll()`, `atof()`, `strtol()`, `strtoll()`, `strtoul()`, ,
`strtoull()`, `strtoimx()`, `strtoumax()`, `strtof()`, `strtod()`, and
`strtold()`.  The `atoX()` family of functions provide _no_ error
detection.  None.  Period.  As such, I consider them useless.  The
`strtoX()` functions do provide error detection.  In addition, they
cover a wider range of numeric types and, for integers, conversion
from a limited number of bases (base 2 - base 36).  Nevertheless,
these functions have their own limitations.

These days there are quite a number of numeric types not directly
covered by `strtol()` et al..  These include:

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
library plus some extra code.  So, rather than cut-and-pate (or
whatever) every time I need to do one of these I've finally just put
them all in `libUtil`.  The corresponding functions are:

    strtos(), strtous(), strtoi(), strtoui(), strtoi8(), strtoui8(),
    strtoi16(), strtoui16(), strtoi32(), strtoui32(), strtoi64(),
    strtoui64(), strtoif8(), strtouif8(), strtoif16(), strtouif16(),
    strtoif32(), strtouif32(), strtoif64(), strtouif64(), strtoif8(),
    strtouil8(), strtoil16(), strtouil16(), strtoil32(), strtouil32(),
    strtoil64(), strtouil64()

All of these take arguments exactly analogous to `strtol()`.  That is,
`strtos()` is really:

    short strtos(const char *Str, char **End, int Base);

and so on.  Error handling semantics, etc. are also exactly analogous.
In fact, for some types, if the `MAX` and `MIN` values are the same,
macros simply reduce the function to a call to the appropriate
function from the standard C library.

[comment]: # (We should "tweak" the .h files, etc. so that, where)
[comment]: # (possible, they reduce to 'inline' functions.)

String to `char` conversion (and relatives) are a special case.  There
are two reasonable ways to convert a string to a character.  The first
is to simply convert it as a `char` sized integer.  That is, a valid
`signed char` might accept strings like "0x0d", "10", "-128", etc..
The second is to accept C-style strings representing a single
character.  For example: "a", "0", "=", "\n", "\t", "\xff", etc..
`libUtil` supports both of these through six functions.  The straight
numeric conversions are:

    strtoc(), strtosc(), strtouc()

while the single character string conversions are:

    strtochar(), strtoschar(), strtouchar()

Again, return values and error semantics are as close to `strtol()` et
al. as makes sense.

WARNING: The complete refusal of the standard to specify a signedness
for `char` makes use of `strtoc()` highly problematic as, depending on
the platform and/or compiler, it may behave like either `strtosc()` or
`strtouc()`.

IMO, the `strtoX()` functions also still leave something to be desired
in terms of error handling and reporting.  While they _do_ report
errors (unlike the `atoX()` functions), the semantics are,
nevertheless, a bit obscure and sometimes highly counter-intuitive.
The `StrToX()` family of functions take care of all this error
checking and return a valid result _only_ if the input string consists
_entirely_ of a value representable as the appropriate type.  For
example, from `StrToL.c`:

    int StrToL(const char *Str, int Base, long *Val)

> Equivalent to `strtol()` but with simplified error reporting.
> Returns the `errno` value rather than setting the global `errno`.
> The global `errno` is always left unchanged.  It is an error not to
> use all of `Str`, or for `(*Str == '\0')`.  Returns `ERANGE` on
> overflow/underflow, `EINVAL` for a malformed `Str`, `EDOM` for an
> invalid `Base`, and `EFAULT` if either `Str` or `Val` is `NULL`.  If
> the function returns non-zero then `*Val` is unchanged.

All other `StrToX()` functions are exactly analogous (except for those
returning floating point types and the `StrToChar()` functions none of
which have a `Base` argument).


### Coding StrToX() Functions

Mostly for my own future edification (and for anyone else with
sufficient curiosity, boredom, whatever) we'll now examine just how
much work/logic is involved in producing these results.  In the end,
it's not a _lot_ of code, it's just that making sure it's correct
requires untangling the error semantics of `strtol()`, et al..  We'll
examine three functions which each adds complexity to the problem.
We'll start with `StrToL()`, progress to `StrToUL()`, and finish with
`StrToUS()`.  The extensions to other types should be "obvious to the
most casual observer". :smiling_imp:  So, here we go!


#### StrToLong()

According to `man strtol` on my Debian GNU/Linux systems (both
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
_anything!_ It might return zero, cause a segfault, quietly corrupt
the heap, etc..  Therefore, the first lines of code in `StrToL()`
check for these kinds of cases and handle them "gracefully":

```C
int StrToL(const char *Str, int Base, long *Val)
{ /* StrToL(const char *, int, long *) */
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

which sounds appropriate.

Now that we know that `(Str != NULL)`, we can deal with the
possibility that `Str` begins with "An initial, possibly [non-]empty,
sequence of white-space characters (as specified by isspace())".
Several "NOTE:"s are in order here. :smiling_imp: NOTES:

1. If you read the Debian GNU/Linux man page you'll find this
   statement: "If `endptr` is not `NULL`, `strtol()` stores the address
   of the first invalid character in `*endptr`.  If there were no
   digits at all, `strtol()` stores the original value of `nptr` in
   `*endptr` (and returns 0)."

   This seems a bit self contradictory.  Suppose `str` is " foo" (and
   assume `base` is 10).  On the one hand, the first invalid character
   if the `'f'` (remember, `strtol()` considers leading white-space as
   valid) so `**endptr'` should be `'f'`.  On the other hand, there are
   "no digits at all" so `(*endptr == str)` seems appropriate.  And
   while I'm at it, what if `str` is " a1"?  The first invalid
   character is `'a'` and there _is_ a digit, so, taking the above
   description literally, `**endptr` _must_ be `'a'`.  But ... really?
   In point of fact, no.  On Debian GNU/Linux `strtol()` returns 0 and sets
   `(*endptr == str)`.

   [The Single Unix Specification][1] is clearer on the subject (though
   a bit long winded):

   "The subject sequence is defined as the longest initial sub-sequence
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
   helpful: "The `isspace()` ... and `isspace_l()` ... functions shall
   test whether `c` is a character of class space in the current locale,
   ...  or in the locale represented by `locale`, ... respectively;".
   Now I admit to being a bit pedantic here, but not knowing any
   better I have to ask whether _somewhere_ there might not be a
   locale where `'\0'` is considered white-space?

With all this ambiguity floating it seems easier to just take matters
into our own hands and check for the empty string or leading white-space
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
special value 0." and, under "ERRORS" it says, "EINVAL (not in C99)
The given base contains an unsupported value.".
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
Wikipedia definition of domain), the "domain" of a function is
"... the domain of definition (or simply the domain) of a function is
the set of "input" or argument values for which the function is
defined.".

Okay, at this point we know that `Str` is not `NULL`, `Val` is not
`NULL`, `Str` is not empty nor does it start with white-space, and
`Base` is valid.  Only a couple more details before we can actually
call `strtol()`.

We want to guarantee that we don't change `errno`.  Since `strtol()`
itself can set `errno` we must save the current value so we can
restore it later, then set it to 0 to detect errors returned from
`strtol()`.  Conversely, on return from `strtol()` we save the value of
`errno` for later use.  In principle we don't need to do this because,
as we'll see, the remainder of the function calls no other functions.
Therefore, we know `errno` won't change on us.  However, it is
conceivable that in some future version that may change.  Saving the
value protects us from that case.  So, next we have:

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
non-whitespace character followed by zero or more other characters.
This in turn implies that `Str` is composed of a (possibly empty) set
of characters we'll call "N" that represent an integer in base `Base`,
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
error because that makes `Str` malformed in the context of `StrToL()`.
If "S" was empty (`Str` was well formed) then it only remains to check
the saved value of `errno` to see whether we had overflow or
underflow.  If so, return the saved `errno`, otherwise restore the
saved `errno`, store the converted value in `*Val`, and return 0:

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
} /* StrToL(const char *, int, long *) */
```

Piece of cake ... not! :frowning:


#### StrToUL()

One would think that, having waded through all the details of writing
`StoToL()`, `StrToUL()` would simply be a matter of calling `stroul()`
at the appropriate place.  Sadly, no such luck. :frowning: While much
of the code is the same, there is one added complication.  From the
Debian GNU/Linux man page for `strtoul()` we have:

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
int StrToUL(const char *Str, int Base, unsigned long *Val)
{ /* StrToUL(const char *, int, unsigned long *) */
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

Now in `StrToL()` this would be the point where we would get ready
to call `strtol()`.  Fortunately, this turns out to be a good place to
check whether 'Str' represents a negative number.  At this point we
already know that `Str` is not `NULL`, is not empty, and does not
start with white-space.  If `Str` at least starts with a well formed
integer then the first character _must_ be either a `'+'`, a `'-'`, or
a valid digit in the selected base.  For a negative value it _must_ be
`'-'`.  So, all we have to do is:

```C
  if (*Str == '-')
  { /* Negative number. */
    return ERANGE;
  } /* Negative number. */
```

We return `ERANGE` because negative numbers are (or _shouldn't_ be
:angry: ) out of the range that can be represented by an unsigned
type.

The remainder of the code is identical to `StrToL()` except that we
call `strtoul()` instead of `strtol()`.


#### StrToS()

Now we really are almost done.  All our work writing the myriad of
`strtoX()` functions pays off.  `StrToS()` is identical to
`StrToL()` except that we call (our own) `strtos()` instead of
`strtol()`.


### String Conversions, Conclusion

It's amazing how hard it has been to get this seemingly simple
functionality right.  In fact, looking back on things, I doubt that
I've ever got it right before.  But then, that's the whole point of
`libUtil`.  Take a bunch of stuff that _should_ be easy but isn't,
wrap it up in code that _does_ make it easy, and stick it in a
library.  For string conversions that's now done ... and I hope never
to have to do it again.  Of course, there's hope, and then there's
reality.  At some point I imagine I'll have to/want to do the
equivalent for `wchar_t` strings starting with `wcstol()` and
friends.  Oh well.


## Dynamic C Strings

The section [Dynamic Strings](#intro_string) gives a fairly good
overview of the functionality of `struct String` (henceforth
`String_t`).  In this section we'll go over some of the design
decisions and implementation details of this "class".  But first ...

WARNING 0: The first thing that should be made clear is that `String_t`
_is_ coded assuming it will be used like a `class` in C++.  That is,
all access to and/or operation on a `String_t` are expected to be
handled by the functions provided for that purpose.  If you mess
directly with members of `String_t` then on your own head be it if
something breaks down the road.

WARNING 1: As a corollary to WARNING 0, it is never safe to pass a
`String_t` by value, to return one (from a function) by value, or even
to assign one to another using any of the C assignment operators.
Such is the price of working in C.  Furthermore, I can not see a way
to implement a dynamically sized string in C that _would_ allow such
things.  So, for the remainder of this section I will consider you to
have been warned and henceforth ignore these issues.

WARNING 2: There are still a number of aspects to the `String_t` API
that I'm considering changing substantially so even if you heed
WARNING 0, things may well break down the road.

WARNING 3: `String_t` is intended to hold a C string.  That
is, a `NUL` terminated string.  If you somehow managed to give it a
"string" containing embedded `NUL` characters, all bets are off.  At
some point I plan to write a `Buffer_t` "class" specifically to hold
arbitrary binary data.  But this isn't it!

Finally, since `String_t` is intended to mimic a C++ "class" (at leas,
as far as is reasonably possible in C) the first argument to most
function is `String_t *This`.  This is analogous to the `this` pointer
in C++ and represents the string being operated on.  The only
exceptions are functions that, in C++, would be "class static".  An
example is:

```C
String_t *StringConcatStr(String_t *Str0, const String_t *Str1);
```
Okay.  With all that out of the way let's take a look at the design of
`String_t`.


### First Attempt

First, let's take a straight forward approach.  The "obvious" thing to
do would be to have `String_t` declared something like this:

```C
struct String
{ /* String */
  size_t Size;
  size_t BuffSize;
  char *Ptr;
}; /* String */
typedef struct String String_t;
```

In this implementation `Ptr` points to the current buffer, `BuffSize`
is the total size of the current buffer in `char`s, and `Size` is the
length of the string currently stored in `Ptr` _not_ including the
termination `NUL`.  While this implementation is easy to understand
it is relatively expensive to initialize/construct.  For example a
default "constructor" might look something like this (for clarity,
I've omitted all error checking):

```C
bool ConstructString(String_t *This) {
  This->Ptr = malloc(1);
  This->Size = 0;
  This->BuffSize = 1;
  This->Ptr[0] = '\0';
  return true;
}
```

The call to `malloc()` is relatively expensive.  To make matters
worse, the first time we assign a non-zero length value we'll have to
call either `malloc()` or `realloc()`, both potentially expensive
operations.  Granted, if the string continues to grow, an exponential
increase in the buffer size will yield "amortized constant time"
performance.  However, common programming wisdom says that most
strings are relatively short so the cost of initialization is probably
significant.

There are several relatively simple ways to eliminate one of the calls
to `malloc()`.  For example, the default constructor could set `Ptr`
to `NULL` instead of calling `malloc()` to allocate space for a single
`char`.  Thus the initial `malloc()` would be eliminated.  This
approach also has the advantage that a `String_t` can easily be
initialized by static initializer:

```C
String_t Str = {
  .Size = 0,
  .BuffSize = 0,
  .Ptr = NULL
};
```  

which could, in turn, be encapsulated in a macro:

```C
#define STRING_T_INITIALIZER {.Size = 0, .BuffSize = 0, .Ptr = NULL }
```

Of course, then every "method" that operates on a `String_t` would
need to check for the case where `(Ptr == NULL)` and handle it
appropriately.

Another approach would be to gratuitously allocate a larger buffer.
We again assume that most strings are fairly short.  So, if we
allocate a small buffer large enough to accommodate most short strings,
then, typically, we won't ever have to call `malloc()`/`realloc()`
again.  (This is a variant of the so-called Short String Optimization.
We'll come back to this shortly.)  Needless to say, like our first
implementation, this cannot be statically initialized.

At any rate, no matter which of the above implementations we select,
we are guaranteed to have to call `malloc()` at least once unless, of
course, we never actually use the object.


### Short String Optimization, Take 1

It would be nice not to have to call `malloc()` or `realloc()` at all
if possible.  Enter the so called Short String Optimization (SSO).
With the SSO we include a short buffer as part of `String_t` itself.
Such a `String_t` might look like this:

```C
#define STRING_MIN_BUFF_SIZE 16

struct String
{ /* String */
  size_t Size;
  size_t BuffSize;
  char *Ptr;
  char Data[STRING_MIN_BUFF_SIZE];
}; /* String */
typedef struct String String_t;
```

Initially, `(Ptr == Data)` and, as long as the contents are never
longer than `STRING_MIN_BUFF_SIZE` we never need to call `malloc()`.
If the contents ever do overflow `Data` then, of course, we will have
to allocate a bigger buffer and `Ptr` would then point to
that ... but we would have had to do this anyway!  So, in many cases
we avoid calling `malloc()` entirely, and even if we do eventually
have to call it we are no worse off than we were to begin with.
Almost.

If we do have to allocate a buffer we still have to carry the local
buffer around with us as wasted space.  Also, this implementation
_can_ be initialized by a static initializer:

```C
String_t Str = {
  .Size = 0,
  .BuffSize = STRING_MIN_BUFF_SIZE,
  .Ptr = Str.Data,
  .Data = { '\0' }
};
```

but note that it requires the name of the object being initialized
(i.e. the `Str` in `.Ptr = Str.Data,`).  This makes encapsulating this
in a macro somewhat more awkward:

```C
#define STRING_T_INITIALIZER(name) { \
  .Size = 0, \
  .BuffSize = STRING_MIN_BUFF_SIZE, \
  .Ptr = name.Data, \
  .Data = { '\0' }
}
```

### Short String Optimization, Take 2

We can reduce the problem of wasted space by noting that as long as
we are using the local buffer we don't need `BuffSize` since we know a
priori that it's value would be `STRING_MIN_BUFF_SIZE`.  On the other
hand if we're using a buffer we got from `malloc()` or `realloc()`
then we don't care about the contents of `Data`.  So, why not combine
them in a `union` (I know: Gag, choke :^| ).  That is:

```C
struct String
{ /* String */
  size_t Size;
  union {
    size_t Size;
    char Data[STRING_MIN_BUFF_SIZE];
  } Buff;
  char *Ptr;
}; /* String */
typedef struct String String_t;
```

This is, in fact, exactly the declaration of `String_t` currently used
in `String.h`.  It reduces the "wasted space" by `sizeof(size_t)`.  On
a 64-bit system this is probably 8, or half the size of the local
buffer.  The is a price for this optimization of course.  If we are
using the local buffer (`Buff.Data`) then `(Ptr == Buff.Data)`.  On
the other hand, if we have had to allocate a buffer on the heap then
`(Ptr != Buff.Data)` and `Buff.Size` holds the size of the buffer.
Dealing with these two cases leads to increased code complexity.

We should point out here that, alternatively, it would be possible to
make the `union` contain `Ptr` and `Data`, while, say, `BuffSize` is a
regular data member.  In this case, if `(BuffSize ==
STRING_MIN_BUFF_SIZE)` then we can assume the string is stored in
`Buff.Data`.  Otherwise, `Buff.Ptr` points to a buffer allocated on the
heap.  I'm not sure which version I'll keep, but the current
implementation is as shown previously.


### The Code

Broadly speaking, the code in `String.c` can be divided into two
sections: Code that handles resizing the buffer, and everything
else. :)  We go over the resizing code first since that is, after all,
what makes `String_t` "dynamic".  Then we briefly review the rest of
the code.  Coincidentally, the resizing code is almost entirely
"static" (i.e. has file scope/visibility) while the rest provides the
globally visible API.

We should also briefly mention error handling.  All functions in
`String.c` return a value that, in one way or another, indicates
whether an error occurred.  A somewhat more specific error code is
returned in `errno`.  These are always standard values defined in
`errno.h`.  Frequently a more detailed error message is printed to
`stdout`.  This is not an ideal solution.  Eventually we would like to
use whatever error handling strategy we develop as part of `libUtil`.
However, as has been noted previously, that part of `libUtil` is
basically non-existent.  Furthermore, most of the cases where we
return an error are reasonably catastrophic (e.g. `malloc()` failed,
we encounter a `NULL` pointer, etc.).  Therefore, for the time being,
printing an error message seems like a reasonable compromise.

Finally, even though the error handling portion of `libUtil` is
mostly non-existent, we do use the `RETURN()` and `EXIT()` macros in
`String.c`.


#### String Resizing

The most critical and, typically, most complicated part of a dynamic
string implementation is resizing the buffer.  `String_t` is no
exception.

As noted in [Dynamic Strings](#intro_string), the size of the buffer
in a `String_t` never automatically gets smaller.  (NOTE: Those
curious enough to read `String.c` will see that such code exists but
has been commented out.  Unless I find sufficient motivation, it will
stay that way.)  Nevertheless, in order to obtain "amortized constant
time" behavior, we can't just always resize the buffer to just fit the
contents every time we need a bigger buffer.

It can be shown that in order to get "amortized constant time" the
buffer size must be increased by a multiplicative constant each time.
That is, it must grow exponentially.  Unfortunately, deciding exactly
what the optimal growth factor should be is essentially impossible as it
depends on a host of factors.  See these discussions for just two
examples: [1][2], [2][3].  However, common choices in many
implementations are 2, 1.5, and the Golden Ratio ((1 + sqrt(5)) / 2)
or about 1.6180).  Since I come from an age when most microprocessors
didn't have an FPU, using the Golden Ratio is out.  With integer math,
both 1.5 and 2 are easy to compute fairly quickly:

```C
/* Multiply 'Int' by 2. */
Int <<= 1;

/* Multiply 'Int' by 1.5. */
Int += Int >> 1;
```

Since multiplication by 2 is slightly simpler I chose that as my
growth factor.  Also, in the event that at some future time I decide
to implement/enable automatic shrinking, division by 2 is easy whereas
division by 1.5 (multiplication by 0.6666... is much harder).
However, as with most stuff about `String_t` at this point, this may
change in the future.  Fortunately, such a change should be
transparent to the end user.

Having established our growth factor, let's consider what needs to be
done in order to implement this.

Even before getting into the details of resizing, one thing should be
apparent given that we chose to implement the SSO.  It will frequently
be necessary to know whether we are currently using the local buffer
`Buff.Data`, or a buffer allocated from the heap.  In order to
reduce typing and the corresponding chance of errors this has been
implemented in the following function:

```C
static inline bool IsLocalBuff(String_t *This)
{ /* IsLocalBuff() */
  return (This->Ptr == This->Buff.Data);
} /* IsLocalBuff() */
```

The logic should be obvious.  Making it `static inline` should make it
as fast as coding it ... well ... in line every time.  Another thing
we'll probably frequently need is the size of the current buffer.  How
we obtain this also depends on whether we are using the local buffer
or not so we also introduce this little function:

```C
static inline size_t BuffSize(String_t *This)
{ /* BuffSize() */
  return (IsLocalBuff(This) ? STRING_MIN_BUFF_SIZE : This->Buff.Size);
} /* BuffSize() */
```

Okay.  So much for the preliminaries.

At fist glance it seems like resizing the buffer should be hardly
worth considering.  If we need more space, multiply the current size
by 2 and call `realloc()`, right?  Sadly, not quite.  There are
actually a number of special cases to consider:

1. Are we currently using the local buffer, or a buffer allocated on
   the heap?

2. Do we need to preserve the current buffer contents or not
   (i.e. `malloc()` vs. `realloc()`)?

3. Are we growing or shrinking the buffer?  (Even though we never
   _automatically_ shrink the buffer, we do implement
   `StringResizeToFit()` which can be called to explicitly shrink the
   buffer.)

4. In case of error we must be able to restore the original state of
   the `String_t`.

In addition, a typical "public method" will come up with a _minimum_
required buffer size.  We then have to apply our doubling algorithm to
come up with an _exact_ size to pass to either `malloc()` or
`realloc()`.  As a result, the core buffer resizing code has been
split into four functions:

* `static bool MallocBuff(String_t *This, size_t MinSize)`

  Allocate a new buffer of size at least `MinSize`.  If `MinSize` is
  less than or equal to the current buffer size this is (a somewhat
  expensive) no-op.  Otherwise, we keep doubling the current buffer
  size until we exceed `MinSize` then allocate (ultimately via
  `malloc()`) a buffer of (exactly) that size.  Not surprisingly, it
  returns `true` on success and `false` otherwise.

* `static bool ReallocBuff(String_t *This, size_t MinSize)`

  Same as `MallocBuff()` except we ultimately call `realloc()`.
  Returns `true` on success and `false` otherwise.

* `static bool MallocBuffExact(String_t *This, size_t NewSize)`

  Call `malloc()` to allocate a new buffer of exactly size `NewSize`,
  and replace the existing buffer with the new on discarding the
  contents of the old buffer.  The new buffer is not initialized in
  any way (not even by writing an initial `NUL` character).  Returns
  `true` on success and `false` otherwise.

* `static bool ReallocBuffExact(String_t *This, size_t NewSize)`

  Call `realloc()` to resize the existing buffer preserving the
  current contents.  Should the call to `realloc()` fail, the old
  buffer is left intact (this is actually a property of `realloc()`
  itself).  Returns `true` on success and `false` otherwise.

We'll consider all four of these in a more-or-less bottom-up manner.
We begin with `MallocBuffExact()`.

##### MallocBuffExact()

We start by declaring local variables we'll need:

```C
static bool MallocBuffExact(String_t *This, size_t NewSize)
{ /* MallocBuffExact() */
  bool _RetVal;
  int ErrNo;
  size_t CurSize;
  char *NewBuff;
```

where:

* `bool _RetVal;`: Since we use `EXIT()` and `RETURN()` we must
  declare `_RetVal` to hold the function's return value.

* `int ErrNo;`: In the event that `malloc()` (or any other standard C
  library function) returns an error, we need to save the value of
  `errno`.

* `size_t CurSize;`: We want to save the current buffer size as
  returned by `BuffSize()`.

* `char *NewBuff;`: The new buffer returned by `malloc()`.

Next we get the current buffer size with:

```C
  /* Get the current buffer size.  NOTE: '(CurSize >=
   * STRING_MIN_BUFF_SIZE)' is always true. */
  CurSize = BuffSize(This);
```

As noted in the comment, we are guaranteed that `CurSize` is never
less than `STRING_MIN_BUFF_SIZE`.  We know this because anytime we set
`This->Buff.Size` we check to see whether it would be less than or
equal to `STRING_MIN_BUFF_SIZE`.  If so, then we set `This->Ptr` to
`This->Buff.Data` (the local buffer) which in turn will cause `BuffSize()`
to return `STRING_MIN_BUFF_SIZE`.

Next we check that we are not attempting to resize the buffer to
smaller than the size of the local buffer
(i.e. `STRING_MIN_BUFF_SIZE`):

```C
  /* Make sure 'NewSize' isn't too small. */
  if (NewSize < STRING_MIN_BUFF_SIZE)
    NewSize = STRING_MIN_BUFF_SIZE;
```

If we are, then we "clamp" `NewSize` to `STRING_MIN_BUFF_SIZE` since,
in this case we will revert to using the local buffer
(`This->Buff.Data`).

Now that we have both the current and new buffer sizes we can check
that we are actually changing the size of the buffer:

```C
  /* See if we actually are changing the size. */
  if (NewSize == CurSize)
  { /* Nothing to do. */
    RETURN(true);
  } /* Nothing to do. */
```

If not, then we simply exit returning `true`.

Okay, so much for the simple stuff.  Since we got this far, we now
_know_ we're changing the buffer size since `(NewSize != CurSize)`.
However, there are two possible cases.  If `(NewSize >
STRING_MIN_BUFF_SIZE)` then we must call `malloc()` since the
requested buffer is bigger than the local buffer.  Otherwise, we do
_not_ call `malloc()` since we will be using the local buffer for the
new buffer.  These two cases correspond to the two halves of the
following if statement:

```C
  if (NewSize > STRING_MIN_BUFF_SIZE)
  { /* Need to call 'malloc()' */
    ...
  } /* Need to call 'malloc()' */
  else
  { /* Need to use 'Buff.Data'. */
    ...
  } /* Need to use 'Buff.Data'. */
```

First consider the case when the `if ()` is true.  Obviously, the
first thing we need to do is allocate the new buffer:

```C
  if (NewSize > STRING_MIN_BUFF_SIZE)
  { /* Need to call 'malloc()' */
    if ((NewBuff = malloc(NewSize)) == NULL)
    { /* Error. */
      ErrNo = errno;
      printf("%s %s %d: malloc(%zu) failed.  %s.\n",
             __FILE__, __func__, __LINE__, NewSize, StrError(ErrNo));
      EXIT(false);
    } /* Error. */
```

A short digression is in order at this point concerning error
handling. As was pointed out earlier, until the error handling portion
of `libUtil` is fleshed out we don't have a lot of flexibility in
handling errors.  Fortunately, for `String_t` almost all errors are
the result of one of two things:

1. Either `malloc()` or `realloc()` fails.  Since this is almost
   certainly a fairly catastrophic problem we simply print a somewhat
   detailed "debug" error message, set `errno` to some reasonable
   value, and return failure for the function.

2. We are passed a `NULL` pointer.  We always consider this to be an
   error so again we simply print an error message, set `errno`, and
   return failure for the function.

So, in the above code, since we will be calling `printf()` which may
itself set `errno`, the first thing we do is save the current value
off `errno`:

```C
      ErrNo = errno;
```

We then print our error message:

```C
      printf("%s %s %d: malloc(%zu) failed.  %s.\n",
             __FILE__, __func__, __LINE__, NewSize, StrError(ErrNo));
```

which identifies the C source file, the function (`MallocBuff()`) and
the line number where the error occurred.  We also print the argument
we passed to `malloc()` (i.e. the amount of memory we are trying to
allocate) and the error message associated with `ErrNo` (the saved
value of `errno`).  Note that we use `StrError()` which is my
thread-safe implementation of `strerror()`.  See
[Error Handling](#error_handling) for more info.

Finally, we call `EXIT(false)`.  Looking ahead to the end of the
function we see:

```C
Exit:
  errno = ErrNo;

Return:
  return _RetVal;
```

which sets the global `errno` with the value we got after `malloc()`
failed, then returns `_RetVal` which is set to the value passed to
`EXIT()` (`false` in this case).

Now that we're done with the error handling digression we can return to
the main logic of the function.

To recap, we know we need to needed to call `malloc()` to allocate a
new buffer because `NewSize` is greater than `STRING_MIN_BUFF_SIZE`
(the size of the local buffer `This->Buff.Data`.  Now, if `malloc()`
succeeds then we have our new buffer but we still need to install it
in the `Sting_t`.  In principle, we just need to get rid of the
existing buffer and make `This->Ptr` point to the new buffer.
Unfortunately, this is complicated by the fact the we may currently be
using the local buffer.  So, we have to check for this case first:

```C
    if (!IsLocalBuff(This))
      free(This->Ptr);
    This->Ptr = NewBuff;
    This->Buff.Size = NewSize;
```

If we're _not_ using the local buffer then we call `free()` to get rid
of the old buffer, set `This->Ptr` to point to the new buffer, and
save the size of the new buffer in `This->Buff.Size`.  And we're
basically done!  We skip over the `else` block, and call `RETURN(true)`
which jumps to `Return:` which in turn returns the value passed to
`RETURN()` (`true` in this case) as the function return value.

The `else` block can only get executed if we are shrinking the buffer
to the size of the local buffer.  Note that we must actually be
reducing the buffer size.  If `(NewSize == STRING_MIN_BUFF_SIZE)` but
we're already using the local buffer then we're not actually changing
the size and we would have simply returned `true` earlier.  Similarly,
we've made sure `(NewSize >= STRING_MIN_BUFF_SIZE)`.  So, _if_ we get
into the `else` block then the current buffer size must be bigger than
the local buffer (hence we're not currently using the local buffer),
and `(NewSize == STRING_MIN_BUFF_SIZE)`.  Therefore, without further
ado we can:

```C
    free(This->Ptr);
    This->Ptr = This->Buff.Data;
```

and once again, we're done!

NOTE: Under no circumstances do we do _anything_ with the data in the
buffer, nor do we do anything with the _string_ size, `This->Size`.
The caller is entirely responsible for this.


##### ReallocBuffExact()

The function `ReallocBuffExact()` is almost exactly like
`MallocBuffExact()` except that, when required, we call `realloc()` to
change the size of the buffer rather calling `malloc()` to allocate a
brand new one.  We also preserve the contents of the existing buffer
as far as possible (if we're shrinking the buffer obviously some data
will be lost).

So, as before, we begin by declaring local variables (including
`_RetVal` for use with `RETURN()` and `EXIT()`), getting the size of
the current buffer (clamped to `STRING_MIN_BUFF_SIZE`), and checking
that we actually have something to do:

```C
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
```

Again, at this point we now know for sure we are changing the size of
the buffer.  Also as before, we check whether we need a buffer on the
heap or are going to be using the local buffer using this `if`
statement:

```C
  if (NewSize > STRING_MIN_BUFF_SIZE)
  { /* Need to call 'malloc()' or 'realloc()' depending. */
    ...
  } /* Need to call 'malloc()' or 'realloc()' depending. */
  else
  { /* Need to use 'Buff.Data'. */
    ...
  } /* Need to use 'Buff.Data'. */
```

The comment for the "true" block of the `if` hints that this is where
things begin to differ.  If `(NewSize > STRING_MIN_BUFF_SIZE)` then we
know we can't use the local buffer as our new buffer.  However, what
we do depends on whether we are _currently_ using the local buffer.
If we _are_ currently using the local buffer then, obviously, we can
not simply call `realloc()` to resize the buffer.  Instead, we must
simulate `realloc()` by calling `malloc()` to allocate the new buffer,
the copy the contents of the local buffer to the new buffer (checking
for errors in the process:

```C
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
```

On the other hand, if we are not currently using the local buffer then
we can call `realloc()` in the expected manner.  `realloc()`
automatically copies the contents of the existing buffer for us.  The
only catch here is that we may be being asked to shrink the buffer to
a size smaller than the existing string.  In this case `realloc()`
effectively truncates the string at the size of the new buffer.
Therefore, we need to put a terminating `NUL` at the end of the new
buffer and adjust `This->Size` accordingly:

```C
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
        This->Buff.Data[NewSize-1] = '\0';
        This->Size = NewSize - 1;
      } /* Truncated. */
    } /* Not using the local buffer. */
```

Note carefully the logic for the case where the string is truncated.
Recall that `This->Size` is the length of the string _not_ including
the terminating `NUL`.  Therefore, in order to avoid truncation,
`NewSize` must be at least one more than `This->Size`.  Which implies
that the string _will_ be truncated if `(NewSize <= This->Size)`.  In
this case, we need to put a `NUL` in the last character of the new
buffer (`NewBuff[NewSize-1] = '\0';`), and set `This->Size` to the new
size less the terminating `NUL` (`This->Size = NewSize - 1;`).

So, one way or the other, we now have a new buffer of size `NewSize`
allocated from the heap with as much of the string as will fit copied
to the new buffer.  We can now install the new buffer in the
`String_t`:

```C
    This->Ptr = NewBuff;
    This->Buff.Size = NewSize;
  } /* Need to call 'malloc()' or 'realloc()' depending. */
```

and we are done with the case where the new buffer is not the local
buffer.

We still have to deal with the case where the new buffer _is_ the
local buffer.  This is handled in the `else` block of `if (NewSize >
STRING_MIN_BUFF_SIZE)`.  Following the same logic used in
`MallocBuffExact()` we also know that the current buffer must _not_ be
the local buffer.  This implies that we have to copy the data from the
current buffer to the local buffer.  Obviously, we only copy as much
as will fit:

```C
  else
  { /* Need to use 'Buff.Data'. */
    /* Copy all the data that will fit, from the current buffer to the
     * local buffer (a la 'realloc()'). */
    memcpy(This->Buff.Data, This->Ptr, NewSize);
```

Unfortunately, the existing _string_ may have be longer than the local
buffer.  If so, then the above call to `memcpy()` has truncated it and
we need to `NUL` terminate it and set `This->Size` appropriately:

```C
    /* See if we truncated the string.  If so, NUL terminate it and
     * set 'This->Size' accordingly. */
    if (NewSize <= This->Size)
    { /* Truncated. */
      NewBuff[NewSize-1] = '\0';
      This->Size = NewSize - 1;
    } /* Truncated. */
```

Finally, we `free()` the current buffer and make `This` use the local
buffer:

```C
    /* Since we're here we know the following: '(CurSize >
     * STRING_MIN_BUFF_SIZE && NewSize == STRING_MIN_BUFF_SIZE)'.
     * Therefore, we can 'free(Buff->Ptr)' without checking. */
    free(This->Ptr);
    This->Ptr = This->Buff.Data;
  } /* Need to use 'Buff.Data'. */
```

and we are done.  We simply call:

```C
  RETURN(true);
```

The return/exit code is identical to that for `MallocBuffExact()`:

```C
Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* ReallocBuffExact() */
```


##### MallocBuff()

We now have two functions for obtaining a buffer of a specific size.
`MallocBuffExact()` obtains an uninitialized buffer while
`ReallocBuffExact()` obtains a buffer of the specified size and, as
far as possible, preserves the contents of the current buffer.
However, in general, we would like the user level `String_t` "methods"
not to have to bother with computing an exact buffer size since this
would entail significant code duplication.  On the other hand, it is
typically fairly straight forward for a user level function to compute
the exact size of it's resulting string.  All we need then is for a
user level function to be able to request a buffer _at least_ as big
as the function's result.  This is the purpose of `MallocBuff()` and
`ReallocBuff()`.

As was noted earlier, these functions currently will never shrink the
buffer.  However, the code was originally written with the ability to
do in mind.  So, for the sake of completeness, we'll the following
discussion will assume we may wish to automatically shrink the
buffer. As such we'll note here that, more-or-less arbitrarily, we
don't shrink the buffer unless the requested minimum sizes is less
than or equal to 1/4 of it's current size.  This prevents repeatedly
appending and removing a single character from causing repeated
resizes.

`MallocBuff()` is the counterpart to `MallocBuffExact()`.  In fact,
`MallocBuff()` simply computes the new actual buffer size given the
minimum size requested by the caller then calls `MallocBuffExact()` to
actually allocate the buffer.

Not surprisingly, we start by declaring some local variables:

```C
static bool MallocBuff(String_t *This, size_t MinSize)
{ /* MallocBuff() */
  bool _RetVal;
  int ErrNo;
  size_t CurSize, NewSize, Temp;
```

where:

* `_RetVal`: As usual, the place where `EXIT()` and `RETURN()` store
  the function return value.

* `ErrNo`: Also as usual, the place we save the global `errno` in case
  of errors.

* `CurSize`: The current buffer size.

* `NewSize`: The new size we want for the buffer.  This will be
  greater than or equal to the `MinSize` argument.

* `Temp`: Temporary variable used in computing `NewSize`.

We start by getting the current buffer size:

```C
  /* Get the current buffer size. */
  CurSize = BuffSize(This);
```

Comparison of the requested buffer size (`MinSize`) and the current
buffer size (`CurSize`) leads to three possible cases:

```C
  if (MinSize > CurSize)
  { /* Grow buffer. */
    ...
  } /* Grow buffer. */
  else if (MinSize <= CurSize >> 2)
  { /* Shrink buffer. */
    ...
  } /* Shrink buffer. */
  /* else, Do nothing. */
```

recalling that we more-or-less arbitrarily decided we will only
actually shrink the buffer if the requested size is less than or equal
to 1/4 the current size.

If we have been asked to grow the buffer then, in accordance with our
chosen growth factor of 2, we simply keep doubling the size until it
is greater than or equal to `MinSize`.  At fist glance it would seem
we could simply do something like this:

```C
  if (MinSize > CurSize)
  { /* Grow buffer. */
    /* We have to be careful here because, in principle, multiplying
     * by 2 might overflow. */
    NewSize = CurSize;
    while ((NewSize <<= 1) < MinSize)
    { /* While not big enough. */
    } /* While not big enough. */
```

Uh oh! It turns out that `simply` is a slight
oversimplification. :smile: What if `NewSize` is greater than
`MAX_SIZE / 2`?  If we then attempt to double `NewSize` the value will
be greater than `MAX_SIZE` (the maximum value representable by a
`size_t`).  In short, the result will overflow.  To maintain at least
some semblance of "correctness" we need to check for this case.  We do
this as follows.  First, we note that if we compute

```C
    Temp = NewSize << 1;
```

using finite integer arithmetic, then this will be mathematically
equivalent to

```
Temp = (2 * NewSize) mod (SIZE_MAX + 1)
```

using infinite integer arithmetic.  Now if `NewSize <= SIZE_MAX / 2`
then everything is fine and this reduces to `Temp = 2 * NewSize`.  On
the other hand, if

```
SIZE_MAX >= NewSize > SIZE_MAX / 2
```

then things become more complicated.  In this case

```
Temp = (2 * NewSize) mod (SIZE_MAX + 1)
```

becomes equivalent to

```
Temp = (2 * NewSize) - (SIZE_MAX + 1)
```

It can be shown that, in this case, `Temp` is always than `NewSize`
(see [Appendix A](#appendix_a)).  So, we can check for overflow with:

```C
      if ((Temp = NewSize << 1) < NewSize)
      { /* Overflow. */
        ...
      } /* Overflow. */
      else
      { /* No overflow. */
        ...
      } /* No overflow. */
```

which begs the question "What do we do if we _do_ have overflow?".

If we detect overflow then the next largest value we can try must be
`SIZE_MAX`.  So on overflow we simply set

```C
        NewSize = SIZE_MAX;
```

Note that this is also guaranteed to terminate the `while (NewSize <
MinSize)` loop since `CurSize` is also a `size_t` and, therefore, can
not be greater than `SIZE_MAX`.

On the other hand, if we don't have overflow, then we simply set

```C
        NewSize = Temp;
```

making `NewSize` equal to twice it's previous value.  So altogether
the size doubling is computed as follows (leaving out some comments):

```C
    NewSize = CurSize;
    while (NewSize < MinSize)
    { /* While not big enough. */
      if ((Temp = NewSize << 1) < NewSize)
      { /* Overflow. */
        NewSize = SIZE_MAX;
      } /* Overflow. */
      else
      { /* No overflow. */
        NewSize = Temp;
      } /* No overflow. */
    } /* While not big enough. */
```

At this point `NewSize` contains the (exact) size of the buffer we
want to allocate. All that's left to do is call `MallocBuffExact()`
and check for errors:

```C
    /* Get new buffer. */
    if (!MallocBuffExact(This, NewSize))
    { /* Error. */
      ErrNo = errno;
      printf("%s %s %d: MallocBuffExact(%zu) failed.  %s.\n",
             __FILE__, __func__, __LINE__, NewSize, StrError(ErrNo));
      EXIT(false);
    } /* Error. */
  } /* Grow buffer. */
```

and, as far as growing the buffer goes, we're done.

As has been previously noted, the code to automatically shrink the
buffer is currently "commented out" by the

```C
#ifdef STRING_ALLOW_SHRINK
  ...
#endif
```

If `STRING_ALLOW_SHRINK` _is_ defined then we will attempt to shrink
the buffer the requested `MinSize` is less than or equal to 1/4 the
current size.  We then repeatedly divide the buffer size by 2 until we
can't make it any smaller without it being less than `MinSize`.  This
is fairly straight forward:

```C
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
```

The only bits that, perhaps, need some additional explanation are:

1. If we enter the `else if` block then we know that `MinSize` is less
   than or equal to `CurSize / 4` (i.e. `CurSize >> 2`) _and_ we have
   already stored `CurSize / 4` in `NewSize`.

2. The `while` loop exits if `NewSize >> 1 <= MinSize`.  Note,
   however, that if we exit because `NewSize >> 1 == MinSize`, we
   will have assigned `NewSize >> 1` to `Temp`, but _not_ updated
   `NewSize` itself.

3. The final `if` is required in order to handle the case where we
   exit because `NewSize >> 1 == MinSize`.

The somewhat convoluted logic of 2. and 3. is necessary because we
allow `MinSize` to be zero and `0 >> 1 == 0`.  Otherwise, the `while`
loop could have been

```C
    while ((Temp = NewSize >> 1) >= MinSize)
    {
      ...
    }
```

but this loop will never exit if `MinSize` is zero.

Well, now we know what size buffer we want so, as before, we call
`MallocBuffExact()`:

```C
    /* Get new buffer. */
    if (!MallocBuffExact(This, NewSize))
    { /* Error. */
      ErrNo = errno;
      printf("%s %s %d: MallocBuffExact(%zu) failed.  %s.\n",
             __FILE__, __func__, __LINE__, NewSize, StrError(ErrNo));
      EXIT(false);
    } /* Error. */
```

and we're done!  All that's left is the return/cleanup code:

```C
  RETURN(true);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* MallocBuff() */
```

##### ReallocBuff()

There is no much to say here.  `ReallocBuff()` is exactly like
`MallocBuff()` except we call `ReallocBuffExact()` instead of
`MallocBuffExact()` with all that implies.

#### The Public Interface

The string resizing functions are, by far, the most complex part of
`String_t`.  They also are the basis for all the functionality offered
by this "class".  Nevertheless, we'll now briefly cover the functions
that make up the public interface to `String_t`.

##### Constructors

A `String_t` cannot be used uninitialized.  Period.  Initialization is
accomplished through a set of "constructor" functions.  Theses are:

* `bool ConstructString(String_t *This)`
  The "default" constructor.  This function is declared in `String.h`
  as being `static inline` so it should be as fast as a macro or,
  possibly even a "static initializer".

* `bool ConstructStringStr(String_t *This, const String_t *Src)`
  "Copy constructor".  In initialize the `String_t` `This` as a copy of
  `Src`.

* `bool ConstructStringCStr(String_t *This, const char *Src)`
  Initialize a `String_t` from C string (i.e. a `char *`).

* `bool ConstructStringChar(String_t *This, char Char, size_t Count)`
  Initialize a `String_t` with `Count` copies of character `Char`.

These functions assume that `This` is uninitialized and behavior is
undefined if `This` has previously passed to one of the constructors.
In other words, none of these functions will properly _re_initialize a
`String_t`.

Note also that these functions do _not_ create or allocate a
`String_t`.  The user must provide a pointer to an existing (but not
yet initialized) `String_t`'.  However, there are a set of
corresponding functions that both allocate and initialize a `String_t`
(analogous to `new String_t()` in C++).  These are:

* `String_t *NewString()`

* `String_t *NewStringStr(const String_t *Src)`

* `String_t *NewStringCStr(const char *Src)`

* `String_t *NewStringChar(char Char, size_t Count)`

all of which are analogous to the corresponding `ConstructStringX()`
functions except they take no `This` argument.  (`This` is allocated
via `malloc()`.)


##### Destructors

Since a `String_t` may allocate memory from the heap it must also be
"destroyed" when it is no longer needed.  This is done by passing it
to:

```C
void DestroyString(String_t *This)
```

or in the case of a `String_t` created by `NewStringX()` by calling:

```C
void DeleteString(String_t *This)
```

So, typical usage might be something like this:

```C
String_t ProgName;

int main(int argc, char **argv) {
  String_t *Arg1;
  ...
  if (!ConstructStringCStr(&ProgName, argv[0]))
  { /* Error */
    ...
  } /* Error */

  if ((Arg1 = NewStringCStr(argv[1])) == NULL)
  { /* Error */
    ...
  } /* Error */
  ...
  DeleteString(Arg1);
  DestroyString(&ProgName);
  ...
  return 0;
}
```

###### Constructor and Destructor Code

Let's briefly examine the code a couple representative constructors
and the destructors.

The default constructor is trivial:

```C
static inline bool ConstructString(String_t *This)
{ /* ConstructString() */
  This->Ptr = This->Buff.Data;
  This->Ptr[0] = '\0';
  This->Size = 0;

  return true;
} /* ConstructString() */
```

We simply initialize `*This` to use the local buffer and make the
buffer contents the empty string, and set the size to zero.  Note
however that, unlike other constructors, it doesn't check for `(This
== NULL)`.

The corresponding `NewString()` is:

```C
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
```

which, like most `NewStringX()` functions, simply allocates a
`String_t` (checking for failure) then calls the corresponding
`ConstructStringX()` function on the newly allocated `String_t`.

Here is a more complicated example:

```C
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
    
  /* Default constructor. This never fails if '(This != NULL)'. */
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
  strcpy(This->Ptr, Src);
  This->Size = SrcSize;
  RETURN(true);

Exit:
  if (Init) DestroyString(This);
  errno = ErrNo;

Return:
  return _RetVal;
} /* ConstructStringCStr() */
```

Note that this and all other constructors return failure (print an
error message, set `errno`, and return `false`) if any of the pointer
arguments are `NULL`.  At any rate, we call the default constructor on
`This`, call `MallocBuff()` to allocate a buffer big enough to hold
`Src` plus the terminating `NUL` (and, as always, checking for
failure), copy `Src` to our buffer, and set `This->Size`.  Note that
in this constructor, on error we call `DestroyString()` if we had
called `ConstructString()`.

The corresponding `NewStringCStr()` looks almost identical to
`NewString()`:

```C
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
```

except that on error we `free(This)` if we had successfully allocated
it.

All the other constructors follow the same pattern.

The destructors are both pretty trivial:

```C
void DestroyString(String_t *This)
{ /* DestroyString() */
  if (This != NULL && !IsLocalBuff(This))
  { /* Exists, and not local buffer. */
    free(This->Ptr);
  } /* Exists, and not local buffer. */
} /* DestroyString() */
```

and

```C
static inline void DeleteString(String_t *This)
{ /* DeleteString() */
  if (This != NULL)
  { /* Exists. */
    DestroyString(This);
    free(This);
  } /* Exists. */
} /* DeleteString() */
```


###### Other Operations

Most other operations (what few there are) are equally trivial.  For
example:

```C
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
```

where we check for `NULL` pointers, allocate an appropriately sized
buff, copy `Src`, and set `This->Size` (checking for errors along the
way).  If there is an error we print an error message, set `errno`,
and return `false`.  Most other functions follow suit.  The two
exceptions are `StringPrinf()` and `StringStreamf()`.  We'll examine
these in slightly more detail.

###### StringPrintf()

`StringPrintf()` is the equivalent of `sprintf()` from the standard C
library and, in fact, does most of it's work by invoking
`vsnprintf()`.  We start with the usual local variables and `NULL`
pointer checking:

```C
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
```

The only thing worthy of note here is that `StringPrintf()` takes a
variable number of arguments and uses a `va_list` to pass to
`vsnprintf()`.  Speaking of which, the next step is to try to print
into the existing buffer:

```C
  CurSize = BuffSize(This);
  
  va_start(ap, Fmt);
  FullSize = vsnprintf(This->Ptr, CurSize, Fmt, ap);
  va_end(ap);
```

If `vsnprintf()` fails for some reason other than buffer overrun it
returns a negative value.  In this case, the state of the buffer is
undefined so we clear `*This`, set `errno`, and return the value
returned by `vsnprintf()`:

```C
  if (FullSize < 0)
  { /* Error (other than buffer overflow). */
    ErrNo = errno;
    
    /* We dunno what state the string is in so we clear it. */
    StringClear(This);

    printf("%s %s %d: vsnprint(This->Ptr, %zu, Fmt, ap) failed.  %s.\n",
           __FILE__,  __func__, __LINE__, CurSize, StrError(ErrNo));
    EXIT(FullSize);
  } /* Error (other than buffer overflow). */
```

In all other cases `vsnprintf()` returns the number of characters
necessary to hold the entire output (not including the terminating
`NUL`).  So, if `(FullSize >= CurSize)` then we allocate a bigger
buffer and try again:

```C
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
```

If `vsnprintf()` _still_ fails (which really should _never_ happen),
we do the appropriate error handling and `EXIT()`:

```C
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
```

otherwise, we're done:

```C
  This->Size = FullSize;
  RETURN(FullSize);

Exit:
  errno = ErrNo;

Return:
  return _RetVal;
} /* StringPrintf() */
```

`StringStreamf()` is exactly analogous to `StringPrintf()` except that
it appends the output to the existing buffer contents which, in turn,
implies that we call `ReallocBuff()` when we need to increase the
buffer size (see `String.c` for the code).


### Dynamic Strings: Conclusion

We've covered the design and implementation of the dynamic string
"class" `struct String` (aka `String_t`) in some detail.  In
particular we've spent a considerable amount of time going over the
buffer resizing functions `MallocBuffExact()`, `ReallocExact()`,
`MallocBuff()`, and `ReallocBuff()` as almost everything else depends
on the correct operation of these functions.  We've also briefly
covered some of the "constructors" and the "destructors", and some of
the "normal" string operations.

Despite the somewhat code-review-like nature of this review,
`String_t` has had relatively little testing.  In large part
`String_t` was written in anticipation of needs for the
[Error Handling](#error_handling) and
[Command Line Arguments](#command_line) portions of `libUtil`.  I'm
sure as those areas are developed bugs in `String_t` will surface.
With luck, this section will help answer the inevitable questions
about "What the heck is _that_ supposed to do?".  I guess we'll see. :smile:


## <a name="error_handling"></a>Error Handling

Some notes on error handling.  As was noted earlier, currently the
only error handling facilities that actually exist are some macros
(`GOTO()`, `EXIT()` and `RETURN()`), and a thread-safe implementation of
`strerror()` (`StrError()`).  These are discussed in some detail below
followed by current thinking about where I'd like this to go in the
future.


#### Macros

First I'll demonstrate the intended usage of the two main macros,
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

I don't see how `RETURN()` and/or `EXIT()` could be of any use here.
In an ideal world, I'd like to come up with some error handling
"stuff" that would be useful no matter what one's coding style is
like.  However, in reality, this will probably never happen.  Even C++
exceptions impose some restrictions (e.g. objects held in "raw"
pointers will not be `delete()`ed during stack unwinding).  So, I'll
certainly make some attempt to make whatever I come up with
more-or-less general purpose, however, since I _am_ primarily writing
`libUtil` for myself I'm not going to loose a lot of sleep if I can't
make it work for other coding styles.


#### `StrError()`

The standard C library provides three functions for getting an error
message associated with an `errno` value.  These are `strerror()`,
`strerror_r()`, and `strerror_l()`.  Sadly (but not surprisingly),
none of these are entirely acceptable.  `strerror()` is, amongst other
things, not thread-safe.  `strerror_r()` is thread-safe but requires
that the caller know the size of the error message in advance which,
it appears, there is no good way to do.  Finally, `strerror_l()` is
identical to `strerror()` except that it is thread-safe ... [sigh]
... but requires that it be passed a `locale_t` along with the `errno`
value.  Ideally, I'd like a thread-safe version of `strerror()`.  That
is, a function that, given _any_ `int`, will return a `const char *`
error message of some sort (even if it's the empty string or something
like "Unknown error nnnn".  This is what `StrError()` does.  NOTE: The
current implementation is a wrapper around `strerror_l()` and, as a
result may return messages of the form "<No error message. ... >" if a
`locale_t` cannot be obtained for the current locale (e.g. if your
locale is environment misconfigured).  This behavior may well change
in the future.


### Future Directions

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

## <a name=command_line></a>Command-line Arguments


## <a name="appendix_a></a>Appendix A

In the size doubling algorithm for the buffer used by `String_t` we
have to check for overflow.  More specifically, in the code in
question, we have to check whether

```
2 * NewSize > SIZE_MAX
```

We wish to check for overflow by showing that if

```
(SIZE_MAX + 1) / 2 <= NewSize <= SIZE_MAX
```

or equivalently, since `NewSize` is an integer

```
(SIZE_MAX + 1) / 2 <= NewSize < (SIZE_MAX + 1)
```

then

```
Temp = NewSize << 1 < NewSize
```

where we assume that

```
Temp = NewSize << 1
```

is equivalent to

```
Temp = (2 * NewSize) mod (SIZE_MAX + 1)
```

NOTE: This has always been intuitively obvious to me, but for once
I'm going to record an actual proof! :smile:

We'll start by considering the general case of

```
z = x mod y
```

where `x mod y` is, to quote the Linux man page, "... the
floating-point remainder of dividing x by y.  The return value is x -
n * y, where n is the quotient of x / y, rounded toward zero to an
integer.".  The cases we will be interested in are:

```
x mod y = x,      0 <= x < y
x mod y = x - y,  y <= x < 2y
```

From here we can now consider what happens if we evaluate `2x mod y`
instead.  By simple substitution we have

```
2x mod y = 2x,      0 <= 2x < y
2x mod y = 2x - y,  y <= 2x < 2y
```

Now let's consider the condition on the first case

```
2x mod y = 2x,      0 <= 2x < y
2x mod y = 2x,      0 <= x < y/2
```

Similarly for the second case we have

```
2x mod y = 2x - y,  y <= 2x < 2y
2x mod y = 2x - y,  y/2 <= x < y
         = x + (x - y),  y/2 <= x < y
```

Clearly `x + (x - y)` will be less than `x` iff

```
(x - y) < 0
x < y
```

But by definition we are only considering the range `y/2 <= x < y` so
`x` _is_ less than `y` and, therefore, `2x mod y < x`.  Q.E.D.


We've proven our conjecture where `x` is a real number.  However, in the
specific case in question, `x` is a `size_t` which is an integer type
capable of representing values `0 <= x < (SIZE_MAX + 1)`.  As noted
earlier

```
x << 1 = 2x mod (SIZE_MAX + 1),  0 <= x < (SIZE_MAX + 1)
```

and therefore

```
2x mod (SIZE_MAX + 1) = x,  0 <= x < (SIZE_MAX + 1) / 2
2x mod (SIZE_MAX + 1) = 2x - (SIZE_MAX + 1),
  (SIZE_MAX + 1) / 2 <= x < (SIZE_MAX + 1)
```

Here we hit our first potential snag.  If `(SIZE_MAX + 1)` is not
even, then `(SIZE_MAX + 1) / 2` is not an integer.  Nevertheless,
since we've proven our results for real numbers within the given
ranges, they must also hold for integers in the same ranges because
the integers are a subset of the real numbers.  So, we can immediately
say that

```
x << 1 = 2x mod (SIZE_MAX + 1) = 2x - (SIZE_MAX + 1) < x
  for (SIZE_MAX + 1) / 2 <= x < (SIZE_MAX + 1)
```

If `(SIZE_MAX + 1)` is even (as is it probably is for the vast
majority of machines) then `(SIZE_MAX + 1) / 2` is also an integer and
is the smallest integer that will cause overflow.  If `(SIZE_MAX + 1)`
is odd, then `(SIZE_MAX + 1) / 2` has a fractional part.  In this
case, the smallest integer that will cause overflow is `(SIZE_MAX + 1)
/ 2` rounded up to the closest integer.  Incidentally, this will also
be `(SIZE_MAX / 2) + 1`.

<!-- Reference links. -->
[1]:(http://pubs.opengroup.org/onlinepubs/9699919799/toc.htm)
[2]:(http://stackoverflow.com/questions/1100311/what-is-the-ideal-growth-rate-for-a-dynamically-allocated-array)
[3]:(https://blog.mozilla.org/nnethercote/2014/11/04/please-grow-your-buffers-exponentially/)
