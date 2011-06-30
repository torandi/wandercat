AC_INIT([wandercat], [1], [ext@sidvind.com])

AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall foreign])
AM_SILENT_RULES([yes])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AM_PATH_SDL

AC_ARG_WITH([fam], [AS_HELP_STRING([--with-fam], [support for File Alteration Monitor@<:@default=disabled@:>@])])
AS_IF([test "x$with_fam" == "xyes"], [
	AC_CHECK_HEADER([fam.h], [
		AC_DEFINE([HAVE_FAM], 1, [Define to 1 if you have FAM])
	])]
)
AM_CONDITIONAL(HAVE_FAM, [test "x$with_fam" == "xyes"])


AC_OUTPUT
