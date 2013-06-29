#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

dnl ---------------------------------------------
dnl May work with pre 2.67, but this is not tested
dnl ---------------------------------------------
AC_PREREQ([2.67])

# A number is attached to the name of the DLL that reflects its LibTool interface version number. The interface of a 
# library is the set of global variables and global functions it exports. It is described by three integers:
#
#    current: The most recent interface number that this library implements.
#    revision: The implementation number of the current interface.
#    age: The difference between the newest and oldest interfaces that this library implements. 
#	  In other words, the library implements all the interface numbers in the range from number current - age to current.
#
#The LibTool version number is usually given in the form current:revision:age. This means that:
#
#    If interfaces have been changed or added, but binary compatibility has been preserved, change to current+1:0:age+1
#    If binary compatibility has been broken, e.g. due to removed or changed interfaces, change to current+1:0:0
#    If the interface is the same as the previous version, change to current:revision+1:age

AC_INIT(libdvdetect library, 0.30.0, bugs@dvdetect.de,,http://www.dvdetect.de)

AC_SUBST(LIBDVDETECT_VERSION_MAJOR, `echo $PACKAGE_VERSION | sed 's/\(.*\)\..*\..*/\1/g'`)
AC_SUBST(LIBDVDETECT_VERSION_MINOR, `echo $PACKAGE_VERSION | sed 's/.*\.\(.*\)\..*/\1/g'`)
AC_SUBST(LIBDVDETECT_VERSION_PATCH, `echo $PACKAGE_VERSION | sed 's/.*\..*\.\(.*\)/\1/g'`)
AC_SUBST(LIBDVDETECT_VERSION_NUMBER, `printf "0x%02x%02x%02x" $LIBDVDETECT_VERSION_MAJOR $LIBDVDETECT_VERSION_MINOR $LIBDVDETECT_VERSION_PATCH`)

AC_SUBST(libdvdetect_la_CURRENT, `echo $LIBDVDETECT_VERSION_MAJOR`)
AC_SUBST(libdvdetect_la_REVISION, `echo $LIBDVDETECT_VERSION_MINOR`)
AC_SUBST(libdvdetect_la_AGE, `echo $LIBDVDETECT_VERSION_PATCH`)

AC_SUBST([OSTYPE])
AM_CONDITIONAL([HAVE_CYGWIN], [test "$OSTYPE" = "cygwin"])

AC_CONFIG_SRCDIR(configure.ac)
#AC_CONFIG_SRCDIR(config.h.in)
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_FILES([Makefile \
                 examples/Makefile \
                 examples/c/Makefile \
                 examples/c++/Makefile \
                 include/Makefile \
                 include/dvdetect/Makefile \
                 include/dvdetect/version.h \
                 lib/Makefile \
                 tests/Makefile])
AC_CHECK_HEADERS([unistd.h sys/socket.h sys/types.h sys/stat.h netinet/in.h arpa/inet.h poll.h netdb.h stdint.h stdbool.h assert.h])                                                                                                  

AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

dnl ---------------------------------------------
dnl windoze stuff
dnl ---------------------------------------------
AM_CONDITIONAL([HAVE_WIN32], [test "$OS" = "Windows_NT"])
AC_CHECK_HEADERS([winsock2.h Ws2tcpip.h])                                                                                                  

dnl ---------------------------------------------
dnl Checks for programs.
dnl ---------------------------------------------
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])
AC_LIBTOOL_WIN32_DLL
AC_PROG_MAKE_SET

dnl ---------------------------------------------
dnl Check if we are running on a big endian machine
dnl ---------------------------------------------
AC_C_BIGENDIAN

dnl ---------------------------------------------
dnl Check for doxygen (dynamic documentation generator)
dnl ---------------------------------------------
AC_CHECK_PROG(DOXYGEN, doxygen, doxygen, no)

AC_OUTPUT
