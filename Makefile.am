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
	examples/c/README \
	examples/c++/README \
	Qt/README \
	Qt/guiexample/guiexample.pro \
	Qt/guiexample/guiexamplestatic.pro \
	Qt/guiexample/main.cpp \
	Qt/guiexample/dvdexplorerdlg.cpp \
	Qt/guiexample/dvdexplorerdlg.h \
	Qt/guiexample/dvdexplorerdlg.ui \
	Qt/guiexample/dvdlanguage.cpp \
	Qt/guiexample/dvdlanguage.h \
	Qt/guiexample/searchtextdlg.cpp \
	Qt/guiexample/searchtextdlg.h \
	Qt/guiexample/searchtextdlg.ui \
	Qt/guiexample/aboutdlg.cpp \
	Qt/guiexample/aboutdlg.h \
	Qt/guiexample/aboutdlg.ui \
	Qt/guiexample/editdetailsdlg.cpp \
	Qt/guiexample/editdetailsdlg.h \
	Qt/guiexample/editdetailsdlg.ui \
	Qt/guiexample/editoptionsdlg.cpp \
	Qt/guiexample/editoptionsdlg.h \
	Qt/guiexample/editoptionsdlg.ui \
	Qt/guiexample/openfromwebdlg.cpp \
	Qt/guiexample/openfromwebdlg.h \
	Qt/guiexample/openfromwebdlg.ui \
	Qt/guiexample/explorerutils.cpp \
	Qt/guiexample/explorerutils.h \
	sql/dbdvdetect.mysql.sql \
	php/inc/functions.inc.php \
	php/inc/functions.query.inc.php \
	php/inc/functions.search.inc.php \
	php/inc/functions.submit.inc.php \
	php/inc/index.php \
	php/index.php \
	php/query.php \
	php/search.php \
	php/submit.php	\
	php/robots.txt \
	win/tinyxml_2_6_2.zip \
	win/readme.txt
	
#	VS2010/dllmain.cpp \
#	VS2010/libdvdetect.sln \
#	VS2010/libdvdetect.vcxproj \
#	VS2010/libdvdetect.vcxproj.filters \
#	VS2010/ReadMe.txt \
#	VS2010/version.h

#: Make documentation via Doxygen http://www.stack.nl/~dimitri/doxygen/
# ${top_srcdir}
# ${srcdir}	
doxygen:
	-( cd ${top_srcdir}/doc && /bin/sh build )
	
	
