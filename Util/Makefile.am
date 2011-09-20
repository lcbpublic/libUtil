## Makefile.am -- Process this file with automake to produce Makefile.in
#AUTOMAKE_OPTIONS = no-dependencies foreign

EXTRA_DIST = acinclude.m4

#AM_CFLAGS = -I..
AM_LDFLAGS =

# Build a libtool library, libUtil.la for installation in libdir.
lib_LTLIBRARIES = libUtil.la
libUtil_la_SOURCES = strtoc.c strtosc.c strtouc.c strtob.c strtoub.c \
	strtos.c strtous.c strtoi.c strtoui.c

libUtil_la_CFLAGS = $(AM_CFLAGS)
#libUtil_la_CXXFLAGS = $(AM_CXXFLAGS)
#libUtil_la_LIBADD = $(db_LIBS) $(gsl_LIBS) $(LIBM)
libUtil_la_LDFLAGS = -no-undefined -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

include_HEADERS = Util.h
# pkginclude_HEADERS = 

$(OBJECTS): libtool
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck
