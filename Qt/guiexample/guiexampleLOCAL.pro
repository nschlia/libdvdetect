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
    ../../win/tinyxml/tinystr.cpp \
    ../../win/tinyxml/tinyxml.cpp \
    ../../win/tinyxml/tinyxmlerror.cpp \
    ../../win/tinyxml/tinyxmlparser.cpp \
    aboutdlg.cpp \
    dvdexplorerdlg.cpp \
    searchtextdlg.cpp \
    ../../lib/xmldoc.cpp \
    ../../lib/md5/md5.c \
    editdetailsdlg.cpp \
    explorerutils.cpp
INCLUDEPATH += \
    ../.. \
    ../../lib \
    ../../include \
    ../../include/dvdetect
win32:INCLUDEPATH += \
    ../../win/tinyxml/ \

HEADERS  += \
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
    ../../win/tinyxml/tinystr.h \
    ../../win/tinyxml/tinyxml.h \
    aboutdlg.h \
    dvdexplorerdlg.h \
    searchtextdlg.h \
    ../../lib/xmldoc.h \
    ../../lib/md5.h \
    ../../lib/vts_ifo.h \
    ../../lib/vmg_ifo.h \
    ../../lib/localutils.h \
    ../../lib/http.h \
    ../../lib/compat.h \
    ../../lib/commonwin32.h \
    ../../lib/common.h \
    ../../lib/md5/md5.h \
    editdetailsdlg.h \
    explorerutils.h

DEFINES += HAVE_CONFIG_H TINYXML_USE_STL TIXML_USE_STL
win32:DEFINES -= UNICODE

CONFIG(debug, debug|release) {
DEFINES += DEBUG
}
else {
DEFINES += NDEBUG
}

win32:LIBS += -lws2_32

#LIBS += -L"$$PWD/../../lib/.libs/" -ldvdetect

FORMS    += \
    dvdexplorerdlg.ui \
    searchtextdlg.ui \
    aboutdlg.ui \
    editdetailsdlg.ui

# SPÄTER WEG!
DEFINES += BUILDING_STATIC

OTHER_FILES += \
    ../../php/test.xml \
    ../../php/submit.php \
    ../../php/search.php \
    ../../php/schema.xsd \
    ../../php/robots.txt \
    ../../php/query.php \
    ../../php/index.php \
    ../../php/inc/functions.inc.php \
    ../../php/inc/index.php \
    ../../sql/dbdvdetect.mysql.sql
