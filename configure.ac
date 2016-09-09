#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.67)

# Some m4 definitions.
# Library version.
m4_define([libUtil_major_version],      [0])
m4_define([libUtil_minor_version],      [0])
m4_define([libUtil_micro_version],      [4])
m4_define([libUtil_version],
          [libUtil_major_version.libUtil_minor_version.libUtil_micro_version])

m4_define([libUtil_interface],          [0])
m4_define([libUtil_interface_age],      [0])
m4_define([libUtil_interface_revision], [4])
#m4_define([libUtil_binary_age],    [0])

# libtool version related macros.  See
# http://localhost/cgi-bin/dwww/usr/share/doc/libtool-doc/libtool.html?type=html#Versioning.
m4_define([libUtil_lt_release],
          [libUtil_major_version.libUtil_minor_version.libUtil_micro_version])
m4_define([libUtil_lt_current], [libUtil_interface])
m4_define([libUtil_lt_age], [libUtil_interface_age])
m4_define([libUtil_lt_revision], [libUtil_interface_revision])

## Required package versions.
## Required gtkmm version.
#m4_define([gtk_pkg], [gtk+-2.0])
#m4_define([gtk_required_version], [2.0.0])

# Start main autoconf stuff.
AC_INIT(libUtil, libUtil_version, lcbpublic@gmail.com, libUtil)
AC_CONFIG_HEADERS(Util/config.h)
AC_CONFIG_SRCDIR([Util/Util.h])
AC_CONFIG_MACRO_DIRS([m4])
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
#AC_PROG_RANLIB
AC_PROG_LIBTOOL
AC_ISC_POSIX
PKG_PROG_PKG_CONFIG

# libUtil version
UTIL_MAJOR_VERSION=libUtil_major_version
UTIL_MINOR_VERSION=libUtil_minor_version
UTIL_MICRO_VERSION=libUtil_micro_version
UTIL_VERSION=libUtil_version

UTIL_INTERFACE=libUtil_interface
UTIL_INTERFACE_AGE=libUtil_interface_age
UTIL_INTERFACE_REVISION=libUtil_interface_revision
#UTIL_BINARY_AGE=libUtil_binary_age

# This is the X.Y used in -llibUtil-FOO-X.Y
#UTIL_API_MAJOR_VERSION=libUtil_api_major_version
#UTIL_API_MINOR_VERSION=libUtil_api_minor_version
#UTIL_API_VERSION=libUtil_api_version
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
LT_RELEASE=libUtil_lt_release
LT_CURRENT=libUtil_lt_current
LT_AGE=libUtil_lt_age
LT_REVISION=libUtil_lt_revision
#LT_CURRENT_MINUS_AGE=libUtil_lt_current_minus_age
AC_SUBST([LT_RELEASE])
AC_SUBST([LT_CURRENT])
AC_SUBST([LT_AGE])
AC_SUBST([LT_REVISION])
#AC_SUBST([LT_CURRENT_MINUS_AGE])

# Libtool initialization.
AC_LIBTOOL_DLOPEN
AC_LIBTOOL_WIN32_DLL
#AC_PROG_LIBTOOL
#AM_PROG_LIBTOOL
AC_SUBST([LIBTOOL_DEPS])
LT_INIT

#if ./libtool --features | grep '^enable static libraries$' > /dev/null 2>&1; then
#  STATIC=-static
#else
#  STATIC=
#fi
#AC_SUBST([STATIC])

#AM_CONDITIONAL([BINARY_HELLDL],
#    [grep '^global_symbol_pipe=..*$' ./libtool >/dev/null])

# Checks for packages.
#PKG_CHECK_MODULES([gtk], [gtk_pkg >= gtk_required_version])
#AC_SUBST(gtk_CFLAGS)
#AC_SUBST(gtk_LIBS)

# Checks for libraries.
#AC_CHECK_LIBM
#AC_SUBST([LIBM])

# Checks for library functions.
AC_FUNC_STRERROR_R
AC_FUNC_MKTIME
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_FUNC_STRTOLD
AC_CHECK_FUNCS([strtol strtoul strtoull strtoull strtof])
AC_CHECK_FUNCS([newlocale freelocale strerror_l])

Util_LIBS=-lUtil
AC_SUBST([Util_LIBS])

# Checks for header files.
AC_CHECK_HEADERS([ctype.h])
AC_CHECK_HEADERS([errno.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([locale.h])
AC_CHECK_HEADERS([stdint.h])
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_STRUCT_TIMEZONE

# Output.
AC_CONFIG_FILES([Makefile
                 Util/Makefile])
AC_OUTPUT
