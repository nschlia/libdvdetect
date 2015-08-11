# dvdetect DVD detection, analysis & DVDETECT lookup library
#
# Copyright (C) 2013 Norbert Schlia <nschlia@dvdetect.de>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

#METASOURCES = AUTO

#AM_CFLAGS = -c -pipe -Wall -Wextra -fexceptions -pedantic-errors
AM_CPPFLAGS = -c -pipe -Wall -Wextra -fexceptions -pedantic-errors
CFLAGS = -O3
CXXFLAGS = -O3
#-g 

DEFS += -DNDEBUG -DTINYXML_USE_STL -DTIXML_USE_STL 

LDFLAGS = 
INCLUDES = 

if HAVE_WIN32
# Some extra stuff required for Windows
LDFLAGS += -lws2_32 --enable-auto-import -fno-keep-inline-dllexport -mthreads
DEFS += -DDLL_EXPORT
INCLUDES += -I${top_srcdir}/win/tinyxml/
else
# Under *nix, tinyxml should be part of the distribution, so simply link
LDFLAGS += -ltinyxml
endif

INCLUDES += -I$(top_srcdir)/include -I$(top_builddir)/include

lib_LTLIBRARIES = libdvdetect.la
libdvdetect_la_SOURCES = \
	dvdcell.cpp \
	dvddatabase.cpp \
	dvdetectbase.cpp \
	dvdetectcglue.cpp \
	dvdfile.cpp \
	dvdparse.cpp \
	dvdpgc.cpp \
	dvdprogram.cpp \
	dvdpttvmg.cpp \
	dvdpttvts.cpp \
	dvdtitle.cpp \
	dvdunit.cpp \
	dvdutils.cpp \
	http.cpp \
	localutils.cpp \
	xmldoc.cpp \
	xmldocparser.cpp \
	xmldocbuilder.cpp \
	libdvdetect.rc

# If no native md5 code, use ours.
if !HAVE_MD5
libdvdetect_la_SOURCES += \
        md5/md5.c
endif

# If no native strptime, use ours.
if !HAVE_STRPTIME
libdvdetect_la_SOURCES += \
        emu/strptime.c
endif

# Windows has no tinyxml, so use our code.
if HAVE_WIN32
nodist_libdvdetect_la_SOURCES = \
	${top_srcdir}/win/tinyxml/tinystr.cpp \
	${top_srcdir}/win/tinyxml/tinyxml.cpp \
	${top_srcdir}/win/tinyxml/tinyxmlerror.cpp \
	${top_srcdir}/win/tinyxml/tinyxmlparser.cpp
endif

libdvdetect_la_LDFLAGS = -no-undefined -version-info $(libdvdetect_la_CURRENT):$(libdvdetect_la_REVISION):$(libdvdetect_la_AGE)

pkginclude_HEADERS = \
	common.h \
	commonwin32.h \
	compat.h \
	http.h \
	localutils.h \
	vmg_ifo.h \
	vts_ifo.h \
	xmldoc.h \
	xmldocbuilder.h \
	xmldocparser.h \
	md5/md5.h
