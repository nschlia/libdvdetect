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

#AM_CPPFLAGS = $(all_includes)
#AM_CFLAGS = -DNDEBUG -c -pipe -fno-keep-inline-dllexport -g -frtti -Wall -Wextra -fexceptions -mthreads
#CPPFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden
#AM_CPPFLAGS = -DNDEBUG -c -pipe -fno-keep-inline-dllexport -g -frtti -Wall -Wextra -fexceptions -mthreads 

#AM_CFLAGS = -c -pipe -g -frtti -Wall -Wextra -fexceptions 
#AM_CPPFLAGS = -c -pipe -g -frtti -Wall -Wextra -fexceptions
AM_CFLAGS = -c -pipe -frtti -Wall -Wextra -fexceptions -pedantic-errors
AM_CPPFLAGS = -c -pipe -frtti -Wall -Wextra -fexceptions -pedantic-errors
#-g -O2 -MT -MD -MP -MF 
CFLAGS = -O2
CXXFLAGS = -O2
#CFLAGS = -g -O2
#CXXFLAGS = -g -O2

DEFS += -DNDEBUG

LDFLAGS =cd  
if HAVE_WIN32
LDFLAGS += -lWs2_32 --enable-auto-import -fno-keep-inline-dllexport -mthreads
#AM_CFLAGS += -DDLL_EXPORT
#AM_CPPFLAGS += -DDLL_EXPORT
DEFS += -DDLL_EXPORT
endif

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

lib_LTLIBRARIES = libdvdetect.la
libdvdetect_la_SOURCES = \
	dvdetectcglue.cpp \
	dvdcell.cpp \
	dvdetectbase.cpp \
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
	localutils.cpp 
	
#libdvdetect_la_CPPFLAGS = -O2
libdvdetect_la_LDFLAGS = -no-undefined -version-info 0:10:0
#libdvdetect_la_LIBADD = $(WS2_32)
#libdvdetect_la_ldflags = -version-info $(libdvdetect_la_CURRENT):$(libdvdetect_la_REVISION):$(libdvdetect_la_AGE) @LT_NO_UNDEFINED@

#METASOURCES = AUTO

pkginclude_HEADERS = \
	common.h \
	commonwin32.h \
	compat.h \
	http.h \
	localutils.h \
	vmg_ifo.h \
	vts_ifo.h
