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

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = dist-bzip2

SUBDIRS	= include lib examples tests

EXTRA_DIST = \
       	doc/doxygen.conf \
       	doc/build \
	examples/README \
	Qt/README \
	Qt/guiexample/guiexample.pro \
	Qt/guiexample/main.cpp \
	Qt/guiexample/dvdexplorerdlg.cpp \
	Qt/guiexample/dvdexplorerdlg.h \
	Qt/guiexample/dvdexplorerdlg.ui 
#	VS2010/dllmain.cpp \
#	VS2010/libdvdetect.sln \
#	VS2010/libdvdetect.vcxproj \
#	VS2010/libdvdetect.vcxproj.filters \
#	VS2010/ReadMe.txt \
#	VS2010/version.h

#: Make documentation via Doxygen http://www.stack.nl/~dimitri/doxygen/
doxygen:
	-( cd ${top_srcdir}/doc && /bin/sh ${srcdir}/build )