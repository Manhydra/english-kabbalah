#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_define([engqbl_version_major], [0])
m4_define([engqbl_version_minor], [3])
m4_define([engqbl_version_build], [0])
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

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h ctype.h string.h getopt.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strdup getopt_long strncasecmp])

dnl  no=do not compile with curl,  yes=compile with curl,  *=error
AC_ARG_WITH(curl,
	[AS_HELP_STRING([--with-curl], [compile with curl to enable remote file access (default=no)])],
	[case "${withval}" in
		yes) curl=yes
			dnl Checks for curl libraries.
			LIBCURL=
			case $host_os in
				dnl WIN32
				*mingw* )
					AC_CHECK_LIB([curldll], [curl_global_init],
						[
							AC_CHECK_HEADERS([curl/curl.h],
								[],
								[AC_MSG_ERROR([Either the header file curl/curl.h cannot be found or is not installed at all. Install curl/curl.h or set --with-curl=no.])]
							)
							AC_SUBST([LIBCURL], ["-lcurldll"])
							AC_DEFINE([HAVE_LIBCURLDLL], [1], [Define if you have libcurldll])
						],
						[AC_MSG_ERROR([Either curl_global_init cannot be found in libcurldll or libcurldll is not installed. Install libcurldll or set --with-curl=no.])]
					)
					;;
				dnl POSIX
					  * )
					AC_CHECK_LIB([curl], [curl_global_init],
						[
							AC_CHECK_HEADERS([curl/curl.h],
								[],
								[AC_MSG_ERROR([Either the header file curl/curl.h cannot be found or is not installed at all. Install curl/curl.h or set --with-curl=no.])]
							)
							AC_SUBST([LIBCURL], ["-lcurl"])
							AC_DEFINE([HAVE_LIBCURL], [1], [Define if you have libcurl])
						],
						[AC_MSG_ERROR([Either curl_global_init cannot be found in libcurl or libcurl is not installed. Install libcurl or set --with-curl=no.])]
					)
					;;
			esac
		;;
		no)  curl=no ;;
		*)   AC_MSG_ERROR(bad value ${withval} for --with-curl) ;;
	esac], [curl=no]
)

dnl  no=do not build eng-qbl,  yes=build eng-qbl,  *=error
AC_ARG_ENABLE(engqbl,
	[AS_HELP_STRING([--enable-engqbl], [build engqbl to create an interactive text shell application (default=no)])],
	[case "${enableval}" in
		yes) engqbl=yes
			dnl Checks for eng-qbl build and readline library.
			LIBREADLINE=
			case $host_os in
				dnl WIN32
				*mingw* )
					AC_CHECK_LIB([readlinedll], [readline],
						[
							AC_CHECK_HEADERS([readline/readline.h readline/history.h],
								[],
								[AC_MSG_ERROR([Either the header files readline/readline.h and readline/history.h cannot be found or is not installed at all. Install readline/readline.h and readline/history.h or set --enable-engqbl=no or --disable-engqbl.])]
							)
							AC_SUBST([LIBREADLINE], ["-lreadlinedll"])
							AC_DEFINE([HAVE_LIBREADLINEDLL], [1], [Define if you have libreadlinedll])
						],
						[AC_MSG_ERROR([Either readline cannot be found in libreadlinedll or libreadlinedll is not installed. Install libreadlinedll or set --enable-engqbl=no or --disable-engqbl.])]
					)
					;;
				dnl POSIX
					  * )
					AC_CHECK_LIB([readline], [readline],
						[
							AC_CHECK_HEADERS([readline/readline.h readline/history.h],
								[],
								[AC_MSG_ERROR([Either the header files readline/readline.h and readline/history.h cannot be found or is not installed at all. Install readline/readline.h and readline/history.h or set --enable-engqbl=no or --disable-engqbl.])]
							)
							AC_SUBST([LIBREADLINE], ["-lreadline"])
							AC_DEFINE([HAVE_LIBREADLINE], [1], [Define if you have libreadline])
						],
						[AC_MSG_ERROR([Either readline cannot be found in libreadline or libreadline is not installed. Install libreadline or set --enable-engqbl=no or --disable-engqbl.])]
					)
					;;
			esac
		;;
		no)  engqbl=no ;;
		*)   AC_MSG_ERROR(bad value ${enableval} for --enable-engqbl) ;;
	esac], [engqbl=no]
)
AM_CONDITIONAL([BUILDENGQBL], [test x$engqbl = x"yes"])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
