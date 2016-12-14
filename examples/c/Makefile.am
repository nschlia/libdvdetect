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
AM_CPPFLAGS = -c -pipe -Wall -Wextra -fexceptions
CFLAGS = -O3
CXXFLAGS = -O3
#-g 

DEFS += -DNDEBUG
LDFLAGS = 

if HAVE_CYGWIN
#DEFS += -DBUILDING_STATIC
else
if BUILDING_STATIC
DEFS += -DBUILDING_STATIC
endif
endif
# The basic API is C++, therefore we need libstdc++ even if using the C API.
LIBDVDETECT_LIBS = -lstdc++

AM_CPPFLAGS += -I$(top_srcdir)/include -I$(top_builddir)/include $(LIBDVDETECT_FLAGS) 

bin_PROGRAMS = dvdinfo dvdlookup dvdsubmit

dvdinfo_SOURCES = dvdinfo.c 
dvdinfo_LDADD = $(top_builddir)/lib/libdvdetect.la $(LIBDVDETECT_LIBS) 

dvdlookup_SOURCES = dvdlookup.c 
dvdlookup_LDADD = $(top_builddir)/lib/libdvdetect.la $(LIBDVDETECT_LIBS) 

dvdsubmit_SOURCES = dvdsubmit.c 
dvdsubmit_LDADD = $(top_builddir)/lib/libdvdetect.la $(LIBDVDETECT_LIBS) 

#pkgincludedir=$(includedir)/dvdetect
pkginclude_HEADERS = 
