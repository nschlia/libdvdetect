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
AM_CFLAGS = -DNDEBUG
#CPPFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden
AM_CPPFLAGS = -DNDEBUG

INCLUDES	= -I$(top_srcdir)/include -I$(top_builddir)/include $(LIBDVDETECT_FLAGS) 

bin_PROGRAMS = dvdinfocxx dvdlookupcxx

dvdinfocxx_SOURCES = dvdinfocxx.cpp
dvdinfocxx_LDADD = $(top_builddir)/lib/libdvdetect.la $(LIBDVDETECT_LIBS) 

dvdlookupcxx_SOURCES = dvdlookupcxx.cpp 
dvdlookupcxx_LDADD = $(top_builddir)/lib/libdvdetect.la $(LIBDVDETECT_LIBS) 

pkginclude_HEADERS = 
