AM_CPPFLAGS =
AM_CPPFLAGS += -DDEBUG -D_GNU_SOURCE
AM_CPPFLAGS += $(libxml2_CFLAGS)
AM_CPPFLAGS += $(libtwsapi_CFLAGS)
AM_CXXFLAGS =
AM_CXXFLAGS += $(DEFAULT_CXXFLAGS)
AM_LDFLAGS =

EXTRA_DIST =
EXTRA_DIST += twsdo.ggo
EXTRA_DIST += twsgen.ggo
EXTRA_DIST += $(BUILT_SOURCES)

headerdir = $(includedir)/twstools

noinst_HEADERS =
header_HEADERS =

bin_PROGRAMS =
bin_PROGRAMS += twsdo twsgen

twsdo_SOURCES =
twsdo_SOURCES += twsdo_main.cpp
twsdo_SOURCES += twsdo.cpp
twsdo_SOURCES += tws_meta.cpp
twsdo_SOURCES += tws_xml.cpp
twsdo_SOURCES += tws_client.cpp
twsdo_SOURCES += tws_query.cpp
twsdo_SOURCES += tws_util.cpp
twsdo_SOURCES += tws_wrapper.cpp
twsdo_SOURCES += tws_quote.cpp
twsdo_SOURCES += tws_account.cpp
twsdo_CPPFLAGS = $(AM_CPPFLAGS)
twsdo_CPPFLAGS += $(INCLTDL)
twsdo_LDFLAGS = $(AM_LDFLAGS)
twsdo_LDFLAGS += $(LIBLTDL) -export-dynamic
twsdo_LDADD =
twsdo_LDADD += $(libxml2_LIBS)
twsdo_LDADD += $(libtwsapi_LIBS)

twsgen_SOURCES =
twsgen_SOURCES += twsgen.cpp
twsgen_SOURCES += tws_xml.cpp
twsgen_SOURCES += tws_meta.cpp
twsgen_SOURCES += tws_query.cpp
twsgen_SOURCES += tws_util.cpp
twsgen_CPPFLAGS = $(AM_CPPFLAGS)
twsgen_LDFLAGS = $(AM_LDFLAGS)
twsgen_LDADD =
twsgen_LDADD += $(libxml2_LIBS)
twsgen_LDADD += $(libtwsapi_LIBS)

noinst_HEADERS += tws_client.h
noinst_HEADERS += tws_wrapper.h
noinst_HEADERS += tws_xml.h
noinst_HEADERS += dso_magic.h

header_HEADERS += debug.h
header_HEADERS += twsdo.h
header_HEADERS += tws_account.h
header_HEADERS += tws_meta.h
header_HEADERS += tws_query.h
header_HEADERS += tws_quote.h
header_HEADERS += tws_util.h

BUILT_SOURCES =
BUILT_SOURCES += twsdo_ggo.c twsdo_ggo.h
BUILT_SOURCES += twsgen_ggo.c twsgen_ggo.h

## example DSO
noinst_LTLIBRARIES =
noinst_LTLIBRARIES += proof_of_concept.la
proof_of_concept_la_SOURCES = proof_of_concept.cpp
proof_of_concept_la_LDFLAGS = -module
## next one is necessary to force the build of a .so
proof_of_concept_la_LDFLAGS += -rpath /none


# Build all executables at distribution time to generate the man pages.
dist-hook: $(bin_PROGRAMS)

CLEANFILES =
CLEANFILES += *.s
CLEANFILES += *.i


%_ggo.c %_ggo.h: %.ggo
	gengetopt --no-help --no-version -i $< -F $*_ggo

# 
# Help the developers get nice post-processed source files

## Create preprocessor output (debugging purposes only)
.c.i:
	$(COMPILE) -E -o $@ $<
.cpp.i:
	$(CXXCOMPILE) -E -o $@ $<

## Create assembler output (debugging purposes only)
.c.s:
	$(COMPILE) -S -c $(cflags) $<
.cpp.s:
	$(CXXCOMPILE) -S -c $(cflags) $<

