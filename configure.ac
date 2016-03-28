#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.67)

# Some m4 definitions.
# Library version.
m4_define([Util_major_version],      [0])
m4_define([Util_minor_version],      [0])
m4_define([Util_micro_version],      [3])
m4_define([Util_version],
          [Util_major_version.Util_minor_version.Util_micro_version])

m4_define([Util_interface],          [0])
m4_define([Util_interface_age],      [0])
m4_define([Util_interface_revision], [3])
#m4_define([Util_binary_age],    [0])

# libtool version related macros.  See
# http://localhost/cgi-bin/dwww/usr/share/doc/libtool-doc/libtool.html?type=html#Versioning.
m4_define([Util_lt_release],
          [Util_major_version.Util_minor_version.Util_micro_version])
m4_define([Util_lt_current], [Util_interface])
m4_define([Util_lt_age], [Util_interface_age])
m4_define([Util_lt_revision], [Util_interface_revision])

## Required package versions.
## Required gtkmm version.
#m4_define([gtk_pkg], [gtk+-2.0])
#m4_define([gtk_required_version], [2.0.0])

# Start main autoconf stuff.
AC_INIT(Util, Util_version, lcbpublic@gmail.com, Util)
AC_CONFIG_SRCDIR([Util/strtoc.c])
#AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE
AC_LANG([C])

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
#AC_PROG_C
#AC_PROG_CCPP
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_LIBTOOL
AC_ISC_POSIX
PKG_PROG_PKG_CONFIG

# Util version
UTIL_MAJOR_VERSION=Util_major_version
UTIL_MINOR_VERSION=Util_minor_version
UTIL_MICRO_VERSION=Util_micro_version
UTIL_VERSION=Util_version

UTIL_INTERFACE=Util_interface
UTIL_INTERFACE_AGE=Util_interface_age
UTIL_INTERFACE_REVISION=Util_interface_revision
#UTIL_BINARY_AGE=Util_binary_age

# This is the X.Y used in -lUtil-FOO-X.Y
#UTIL_API_MAJOR_VERSION=Util_api_major_version
#UTIL_API_MINOR_VERSION=Util_api_minor_version
#UTIL_API_VERSION=Util_api_version
#API_MJ=$UTIL_API_MAJOR_VERSION
#API_MI=$UTIL_API_MINOR_VERSION
AC_SUBST([UTIL_MAJOR_VERSION])
AC_SUBST([UTIL_MINOR_VERSION])
AC_SUBST([UTIL_MICRO_VERSION])
AC_SUBST([UTIL_VERSION])
AC_SUBST([UTIL_INTERFACE])
AC_SUBST([UTIL_INTERFACE_AGE])
AC_SUBST([UTIL_INTERFACE_REVISION])
#AC_SUBST([UTIL_BINARY_AGE])
#AC_SUBST([UTIL_API_MAJOR_VERSION])
#AC_SUBST([UTIL_API_MINOR_VERSION])
#AC_SUBST([UTIL_API_VERSION])
#AC_SUBST([API_MJ])
#AC_SUBST([API_MI])

# libtool versioning
LT_RELEASE=Util_lt_release
LT_CURRENT=Util_lt_current
LT_AGE=Util_lt_age
LT_REVISION=Util_lt_revision
#LT_CURRENT_MINUS_AGE=Util_lt_current_minus_age
AC_SUBST([LT_RELEASE])
AC_SUBST([LT_CURRENT])
AC_SUBST([LT_AGE])
AC_SUBST([LT_REVISION])
#AC_SUBST([LT_CURRENT_MINUS_AGE])

# Libtool initialization.
AC_LIBTOOL_DLOPEN
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
#AM_PROG_LIBTOOL
AC_SUBST([LIBTOOL_DEPS])
LT_INIT

if ./libtool --features | grep '^enable static libraries$' > /dev/null 2>&1; then
  STATIC=-static
else
  STATIC=
fi
AC_SUBST([STATIC])

AM_CONDITIONAL([BINARY_HELLDL],
    [grep '^global_symbol_pipe=..*$' ./libtool >/dev/null])

# Checks for packages.
#PKG_CHECK_MODULES([gtk], [gtk_pkg >= gtk_required_version])
#AC_SUBST(gtk_CFLAGS)
#AC_SUBST(gtk_LIBS)

# Checks for libraries.
#AC_CHECK_LIBM
#AC_SUBST([LIBM])

# Checks for library functions.
AC_FUNC_MKTIME
AC_FUNC_STRTOD
AC_CHECK_FUNCS([strtol strtoul strtoull strtoull strtof strtod strtold])

Util_LIBS=-lUtil
AC_SUBST([Util_LIBS])

# Checks for header files.
AC_CHECK_HEADERS([ctype.h])
AC_CHECK_HEADERS([errno.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([stdint.h])
AC_CHECK_HEADERS([sys/stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_STRUCT_TIMEZONE

# Output.
AC_CONFIG_FILES([Makefile
                 Util/Makefile])
AC_OUTPUT
