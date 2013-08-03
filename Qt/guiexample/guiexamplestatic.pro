#-------------------------------------------------
#
# Project created by QtCreator 2013-05-15T21:02:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = guiexample
TEMPLATE = app

SOURCES += main.cpp \
    aboutdlg.cpp \
    dvdexplorerdlg.cpp \
    searchtextdlg.cpp \ 
    explorerutils.cpp \
    editdetailsdlg.cpp \ 
    editoptionsdlg.cpp \
    ../../lib/dvdcell.cpp \
    ../../lib/dvddatabase.cpp \
    ../../lib/dvdetectbase.cpp \
    ../../lib/dvdetectcglue.cpp \
    ../../lib/dvdfile.cpp \
    ../../lib/dvdparse.cpp \
    ../../lib/dvdpgc.cpp \
    ../../lib/dvdprogram.cpp \
    ../../lib/dvdpttvmg.cpp \
    ../../lib/dvdpttvts.cpp \
    ../../lib/dvdtitle.cpp \
    ../../lib/dvdunit.cpp \
    ../../lib/dvdutils.cpp \
    ../../lib/http.cpp \
    ../../lib/localutils.cpp \
    ../../lib/xmldoc.cpp \
    ../../lib/xmldocbuilder.cpp \
    ../../lib/xmldocparser.cpp

win32:SOURCES += \
    ../../win/tinyxml/tinystr.cpp \
    ../../win/tinyxml/tinyxml.cpp \
    ../../win/tinyxml/tinyxmlerror.cpp \
    ../../win/tinyxml/tinyxmlparser.cpp \
    ../../lib/md5/md5.c \
    ../../lib/emu/strptime.c

INCLUDEPATH += \
    ../.. \
    ../../lib \
    ../../include \
    ../../include/dvdetect
win32:INCLUDEPATH += \
    ../../win/tinyxml/ \

HEADERS  += \
    aboutdlg.h \
    dvdexplorerdlg.h \
    searchtextdlg.h \ 
    ../../config.h \
    ../../include/dvdetect/dvdcell.h \
    ../../include/dvdetect/dvddatabase.h \
    ../../include/dvdetect/dvdetect.h \
    ../../include/dvdetect/dvdetectbase.h \
    ../../include/dvdetect/dvdetectc++.h \
    ../../include/dvdetect/dvdetectdll.h \
    ../../include/dvdetect/dvdfile.h \
    ../../include/dvdetect/dvdparse.h \
    ../../include/dvdetect/dvdpgc.h \
    ../../include/dvdetect/dvdprogram.h \
    ../../include/dvdetect/dvdpttvmg.h \
    ../../include/dvdetect/dvdpttvts.h \
    ../../include/dvdetect/dvdtitle.h \
    ../../include/dvdetect/dvdunit.h \
    ../../include/dvdetect/dvdutils.h \
    ../../include/dvdetect/types.h \
    ../../include/dvdetect/version.h \
    explorerutils.h \
    editdetailsdlg.h \
    editoptionsdlg.h \
    ../../lib/xmldoc.h \
    ../../lib/xmldocbuilder.h \
    ../../lib/xmldocparser.h \
    ../../lib/vts_ifo.h \
    ../../lib/vmg_ifo.h \
    ../../lib/localutils.h \
    ../../lib/http.h \
    ../../lib/compat.h \
    ../../lib/commonwin32.h \
    ../../lib/common.h \
    ../../lib/md5/md5.h \
    ../../include/dvdetect/vector_ptr.h

win32:HEADERS  += \
    ../../win/tinyxml/tinystr.h \
    ../../win/tinyxml/tinyxml.h \

DEFINES += HAVE_CONFIG_H TINYXML_USE_STL TIXML_USE_STL
win32:DEFINES -= UNICODE

CONFIG(debug, debug|release) {
DEFINES += DEBUG
}
else {
DEFINES += NDEBUG
}

win32:LIBS += -lws2_32
else:LIBS += -lcrypto -ltinyxml

#LIBS += -L"$$PWD/../../lib/.libs/" -ldvdetect

FORMS    += \
    dvdexplorerdlg.ui \
    searchtextdlg.ui \
    aboutdlg.ui \
    editdetailsdlg.ui \
    editoptionsdlg.ui

# SPÄTER WEG!
DEFINES += BUILDING_STATIC

OTHER_FILES += \
    ../../php/test.xml \
    ../../sql/dbdvdetect.mysql.sql \
    ../../php/robots.txt \
    ../../php/index.php \
    ../../php/query.php \
    ../../php/search.php \
    ../../php/submit.php \
    ../../php/inc/index.php \
    ../../php/inc/functions.inc.php \
    ../../php/inc/functions.query.inc.php \
    ../../php/inc/functions.submit.inc.php \
    ../../php/inc/functions.search.inc.php \
    ../../php/inc/functions.buildxml.inc.php \
    ../../include/dvdetect/version.h.in \
    ../../THANKS \
    ../../README \
    ../../NEWS \
    ../../COPYING \
    ../../INSTALL \
    ../../AUTHORS \
    ../../ChangeLog
