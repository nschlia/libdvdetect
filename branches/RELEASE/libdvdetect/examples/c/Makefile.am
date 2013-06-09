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
#CPPFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden

AM_CFLAGS = -pipe -Wall -Wextra -fexceptions  
AM_CPPFLAGS = -pipe -Wall -Wextra -fexceptions 
CFLAGS = -O2
CXXFLAGS = -O2
#CFLAGS = -g -O2
#CXXFLAGS = -g -O2

DEFS += -DNDEBUG

INCLUDES	= -I$(top_srcdir)/include -I$(top_builddir)/include $(LIBDVDETECT_FLAGS) 

bin_PROGRAMS = dvdinfo dvdlookup

dvdinfo_SOURCES = dvdinfo.c 
dvdinfo_LDADD = $(top_builddir)/lib/libdvdetect.la $(LIBDVDETECT_LIBS) 

dvdlookup_SOURCES = dvdlookup.c 
dvdlookup_LDADD = $(top_builddir)/lib/libdvdetect.la $(LIBDVDETECT_LIBS) 

pkginclude_HEADERS = 
