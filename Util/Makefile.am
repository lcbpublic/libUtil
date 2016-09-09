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
	\
	StrToChar.c StrToSChar.c StrToUChar.c \
	StrToSC.c StrToUC.c StrToS.c StrToUS.c StrToI.c StrToUI.c \
	StrToL.c StrToUL.c StrToLL.c StrToULL.c StrToF.c StrToD.c StrToLD.c \
	StrToI8.c StrToUI8.c StrToI16.c StrToUI16.c \
	StrToI32.c StrToUI32.c StrToI64.c StrToUI64.c StrToIMax.c StrToUMax.c \
	StrToIL8.c StrToUIL8.c StrToIL16.c StrToUIL16.c \
	StrToIL32.c StrToUIL32.c StrToIL64.c StrToUIL64.c \
	StrToIF8.c StrToUIF8.c StrToIF16.c StrToUIF16.c \
	StrToIF32.c StrToUIF32.c StrToIF64.c StrToUIF64.c \
	\
	String.c Errors.c

libUtil_la_CFLAGS = $(AM_CFLAGS)
#libUtil_la_CXXFLAGS = $(AM_CXXFLAGS)
#libUtil_la_LIBADD = $(db_LIBS) $(gsl_LIBS) $(LIBM)
libUtil_la_LDFLAGS = -no-undefined -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

include_HEADERS = Util.h Errors.h StrToX.h String.h
# pkginclude_HEADERS = 

$(OBJECTS): libtool
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck
