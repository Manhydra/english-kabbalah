#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_define([engqbl_version_major], [0])
m4_define([engqbl_version_minor], [1])
m4_define([engqbl_version_build], [0])
m4_define([engqbl_version_suffix], [])
m4_define([engqbl_version], [engqbl_version_major.engqbl_version_minor.engqbl_version_build])
m4_define([engqbl_version_display], engqbl_version[]m4_ifdef([engqbl_version_suffix],[engqbl_version_suffix]))

AC_INIT([English Kabbalah], [engqbl_version_display], [marc.sylvestre@manhydra.com], [eng-qbl])
AM_INIT_AUTOMAKE([-Wall])
AC_PREREQ([2.67])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h getopt.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strdup])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
