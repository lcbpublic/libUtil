## Makefile.am -- Process this file with automake to produce Makefile.in
#AUTOMAKE_OPTIONS = no-dependencies foreign

EXTRA_DIST = acinclude.m4

#AM_CFLAGS = -I..
AM_LDFLAGS =

# Build a libtool library, libUtil.la for installation in libdir.
lib_LTLIBRARIES = libUtil.la
libUtil_la_SOURCES = strtochar.c strtoschar.c strtouchar.c \
	strtosc.c strtouc.c strtos.c strtous.c strtoi.c strtoui.c \
	strtoi8.c strtoui8.c strtoi16.c strtoui16.c \
	strtoi32.c strtoui32.c strtoi64.c strtoui64.c strtoil8.c strtouil8.c \
	strtoil16.c strtouil16.c strtoil32.c strtouil32.c \
	strtoil64.c strtouil64.c strtoif8.c strtouif8.c \
	strtoif16.c strtouif16.c strtoif32.c strtouif32.c \
	strtoif64.c strtouif64.c \
	StrToEscChar.c StrToEscSChar.c StrToEscUChar.c \
	StrToSChar.c StrToUChar.c StrToShort.c StrToUShort.c \
	StrToInt.c StrToUInt.c StrToLong.c StrToULong.c \
	StrToLongLong.c StrToULongLong.c \
	StrToFloat.c StrToDouble.c StrToLongDouble.c \
	StrToInt8.c StrToUInt8.c StrToInt16.c StrToUInt16.c \
	StrToInt32.c StrToUInt32.c StrToInt64.c StrToUInt64.c \
	StrToLInt8.c StrToULInt8.c StrToLInt16.c StrToULInt16.c \
	StrToLInt32.c StrToULInt32.c StrToLInt64.c StrToULInt64.c \
	StrToFInt8.c StrToUFInt8.c StrToFInt16.c StrToUFInt16.c \
	StrToFInt32.c StrToUFInt32.c StrToFInt64.c StrToUFInt64.c \
	\
	String.c

libUtil_la_CFLAGS = $(AM_CFLAGS)
#libUtil_la_CXXFLAGS = $(AM_CXXFLAGS)
#libUtil_la_LIBADD = $(db_LIBS) $(gsl_LIBS) $(LIBM)
libUtil_la_LDFLAGS = -no-undefined -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

include_HEADERS = Util.h Errors.h StrToX.h String.h
# pkginclude_HEADERS = 

$(OBJECTS): libtool
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck
