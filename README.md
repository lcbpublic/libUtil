# The 'libUtil' utility library

A C library of miscellaneous utilities.  That is, stuff. :)

Author: Leland Best <lcbpublic@gmail.com>

License: GNU GPL v3 or later.  See the file 'COPYING.txt' in this directory.

## Introduction

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

2. A dynamic string "class" ('struct String' aka 'String\_t').

3. Error handling.

4. Command-line argument parsing.

At this point, only the first is really code complete and had any
testing at all.  The second has the basics for 'String\_t' but it needs a
_lot_ more functionality and testing.  And I'm not even certain I'll
keep the present interface.  Number three currently consists of a
couple macros and a lot of ideas but that's it.  Number 4 consists
entirely of thoughts about the shortcomings of existing
libraries/packages and ideas about how to fix them, but that's it.

The rest of this document first briefly addresses the build system,
then documents the existing code and the rationale behind it, and
discusses my thoughts and ideas for stuff that still needs to be
developed.

## Building

## String Conversions

## Dynamic C Strings

## Error Handling

## Command-line Arguments
