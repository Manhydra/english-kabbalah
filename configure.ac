#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_define([engqbl_version_major], [0])
m4_define([engqbl_version_minor], [1])
m4_define([engqbl_version_build], [2])
m4_define([engqbl_version_suffix], [])
m4_define([engqbl_version], [engqbl_version_major.engqbl_version_minor.engqbl_version_build])
m4_define([engqbl_version_display], engqbl_version[]m4_ifdef([engqbl_version_suffix],[engqbl_version_suffix]))

AC_INIT([English Kabbalah], [engqbl_version_display], [marc.sylvestre@manhydra.com], [eng-qbl], [http://www.manhydra.com/projects/english-kabbalah])
AM_INIT_AUTOMAKE([-Wall])
AC_PREREQ([2.67])
AC_LANG([C])
AC_CONFIG_SRCDIR([processing.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Check for host.
AC_CANONICAL_HOST

# Checks for libraries.
case $host_os in
	dnl WIN32
	*mingw* ) AC_CHECK_LIB([curldll], [curl_global_init]) ;;
	dnl POSIX
		  * ) AC_CHECK_LIB([curl], [curl_global_init]) ;;
esac

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h getopt.h curl/curl.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strdup getopt_long])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
