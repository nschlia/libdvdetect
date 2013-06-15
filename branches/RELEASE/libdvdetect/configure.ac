#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

dnl ---------------------------------------------
dnl May work with pre 2.67, but this is not tested
dnl ---------------------------------------------
AC_PREREQ([2.67])
AC_INIT(libdvdetect library, 0.21, bugs@dvdetect.de,,http://www.dvdetect.de)
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
AC_CHECK_HEADERS([unistd.h sys/socket.h sys/types.h sys/stat.h netinet/in.h arpa/inet.h poll.h netdb.h stdint.h stdbool.h])                                                                                                  

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
