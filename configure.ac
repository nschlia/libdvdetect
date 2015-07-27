#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

dnl ---------------------------------------------
dnl May work with pre 2.67, but this is not tested
dnl ---------------------------------------------
AC_PREREQ([2.67])

dnl ---------------------------------------------
dnl Versioning
dnl ---------------------------------------------

# A number is attached to the name of the DLL that reflects its LibTool interface version number. The interface of a 
# library is the set of global variables and global functions it exports. It is described by three integers:
#
#    current: The most recent interface number that this library implements.
#    revision: The implementation number of the current interface.
#    age: The difference between the newest and oldest interfaces that this library implements. 
#	  In other words, the library implements all the interface numbers in the range from number current - age to current.
#
# The LibTool version number is usually given in the form current:revision:age. This means that:
#
#    If interfaces have been changed or added, but binary compatibility has been preserved, change to current+1:0:age+1
#    If binary compatibility has been broken, e.g. due to removed or changed interfaces, change to current+1:0:0
#    If the interface is the same as the previous version, change to current:revision+1:age

#AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_INIT(libdvdetect library, 0.70.0, bugs@dvdetect.de,,http://www.dvdetect.de)

AC_SUBST(LIBDVDETECT_VERSION_MAJOR, `echo $PACKAGE_VERSION | sed 's/\(.*\)\..*\..*/\1/g'`)
AC_SUBST(LIBDVDETECT_VERSION_MINOR, `echo $PACKAGE_VERSION | sed 's/.*\.\(.*\)\..*/\1/g'`)
AC_SUBST(LIBDVDETECT_VERSION_PATCH, `echo $PACKAGE_VERSION | sed 's/.*\..*\.\(.*\)/\1/g'`)
AC_SUBST(LIBDVDETECT_VERSION_NUMBER, `printf "0x%02x%02x%02x" $LIBDVDETECT_VERSION_MAJOR $LIBDVDETECT_VERSION_MINOR $LIBDVDETECT_VERSION_PATCH`)

AC_SUBST(libdvdetect_la_CURRENT, `echo $LIBDVDETECT_VERSION_MAJOR`)
AC_SUBST(libdvdetect_la_REVISION, `echo $LIBDVDETECT_VERSION_MINOR`)
AC_SUBST(libdvdetect_la_AGE, `echo $LIBDVDETECT_VERSION_PATCH`)

dnl ---------------------------------------------
dnl Makefile setup
dnl ---------------------------------------------

AC_CONFIG_SRCDIR(configure.ac)
#AC_CONFIG_SRCDIR(config.h.in)
AC_CONFIG_HEADERS(config.h)

dnl ---------------------------------------------
dnl See if system has md5 support. If not, use our code.
dnl ---------------------------------------------
AC_CHECK_LIB([crypto],[MD5_Init],,[AC_MSG_WARN([No md5 support on system. Fallback to using our own emulation.])])
AC_CHECK_LIB([crypto],[MD5_Init],[AC_SUBST([MD5AVAILABLE],[1])],[AC_SUBST([MD5AVAILABLE],[0])])
AM_CONDITIONAL([HAVE_MD5], [test "$MD5AVAILABLE" = "1"])

dnl ---------------------------------------------
dnl Checks for functions.
dnl ---------------------------------------------
AC_CHECK_FUNCS(strptime,,[AC_MSG_WARN([No strptime on system. Fallback to using our own emulation.])])
AC_CHECK_FUNCS(strptime,[AC_SUBST([STRPTIMEAVAILABLE],[1])],[AC_SUBST([STRPTIMEAVAILABLE],[0])])
AM_CONDITIONAL([HAVE_STRPTIME], [test "$STRPTIMEAVAILABLE" = "1"])

dnl ---------------------------------------------
dnl Distribution archive
dnl ---------------------------------------------
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

dnl ---------------------------------------------
dnl Check for cygwin
dnl ---------------------------------------------
AC_SUBST([OSTYPE])
AM_CONDITIONAL([HAVE_CYGWIN], [test "$OSTYPE" = "cygwin"])

dnl ---------------------------------------------
dnl windoze stuff
dnl ---------------------------------------------
AM_CONDITIONAL([HAVE_WIN32], [test "$OS" = "Windows_NT"])
AC_CHECK_HEADERS([windows.h winsock2.h Ws2tcpip.h], [], [],
     [[#ifdef HAVE_WINDOWS_H
     # include <windows.h>
     #endif
     ]])                                                                                                  

dnl ---------------------------------------------
dnl Checks for programs.
dnl ---------------------------------------------
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
LT_INIT
AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])
AC_LIBTOOL_WIN32_DLL

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lQtCore':
#AC_CHECK_LIB([QtCore], [main])
# FIXME: Replace `main' with a function in `-lQtGui':
#AC_CHECK_LIB([QtGui], [main])
# FIXME: Replace `main' with a function in `-lcrypto':
#AC_CHECK_LIB([crypto], [main])
# FIXME: Replace `main' with a function in `-lpthread':
#AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-ltinyxml':
#AC_CHECK_LIB([tinyxml], [main], [], [
#  AC_MSG_ERROR([unable to find the cos() function])
#])
# FIXME: Replace `main' with a function in `-lws2_32':
#AC_CHECK_LIB([ws2_32], [main])

dnl ==================
dnl Checks for tinyxml
dnl ==================
AC_CHECK_LIB([tinyxml],
             [main],
             [have_tinyxml_library=true],
	     [AC_MSG_WARN([tinyxml library not found.])])
AC_LANG_PUSH([C++])
AC_CHECK_HEADER([tinyxml.h],
                 [have_tinyxml_header=true],
                 [AC_MSG_WARN([tinyxml library headers not found.])],
                 [])
AC_LANG_POP([C++])
if test "x$have_tinyxml_library" != "xtrue" -o "x$have_tinyxml_header" != "xtrue"
then
	if test "$OS" != "Windows_NT" 
	then
     		AC_MSG_ERROR([Please install tinyxml2!])
	fi
fi

dnl ---------------------------------------------
dnl Check if we are running on a big endian machine
dnl ---------------------------------------------
AC_C_BIGENDIAN

# Checks for header files.
AC_CHECK_HEADERS([limits.h stddef.h unistd.h sys/socket.h sys/types.h sys/stat.h netinet/in.h arpa/inet.h poll.h netdb.h stdint.h stdbool.h assert.h openssl/md5.h])                                                                                                  

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MKTIME
# http://rickfoosusa.blogspot.de/2011/11/howto-fix-undefined-reference-to.html
#AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove memset select setenv setlocale socket strchr strdup strerror strncasecmp strrchr strstr strtoul tzset])

AC_CONFIG_FILES([Makefile \
                 examples/Makefile \
                 examples/c/Makefile \
                 examples/c++/Makefile \
                 include/Makefile \
                 include/dvdetect/Makefile \
                 include/dvdetect/version.h \
                 lib/Makefile \
                 tests/Makefile])

dnl ---------------------------------------------
dnl Check for doxygen (dynamic documentation generator)
dnl ---------------------------------------------
AC_CHECK_PROG(DOXYGEN, doxygen, doxygen, no)

AC_OUTPUT
