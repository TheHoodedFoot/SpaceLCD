dnl Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([spacelcd], [2020.2.29], [thf@thehoodedfoot.com])
AC_CONFIG_SRCDIR([src/libspacelcd.c])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

dnl Checks for programs
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AR
LT_INIT

dnl Checks for libraries
PKG_CHECK_MODULES([ZLIB], [zlib])
PKG_CHECK_MODULES([LIBRSVG], [librsvg-2.0])
PKG_CHECK_MODULES([LIBUSB], [libusb-1.0])

dnl Optional: SWIG + Python bindings
AC_PATH_PROG([SWIG], [swig])
AC_PATH_PROG([PYTHON3], [python3])
AC_ARG_ENABLE([python],
  [AS_HELP_STRING([--enable-python], [build Python bindings (default: auto)])],
  [enable_python=$enableval],
  [enable_python=auto])

AM_CONDITIONAL([HAVE_SWIG], [test -n "$SWIG" -a -n "$PYTHON3" -a "x$enable_python" != "xno"])
if test "x$enable_python" = "xyes" -o "x$enable_python" = "xauto"; then
  if test -n "$SWIG" -a -n "$PYTHON3"; then
    # Use sysconfig (Python 3.11+); distutils.sysconfig was removed in Python 3.12
    PYTHON3_CPPFLAGS=`$PYTHON3 -c "import sysconfig; print('-I' + sysconfig.get_path('include'))" 2>/dev/null || echo ""`
    PYTHON3_LDFLAGS=`$PYTHON3 -c "import sysconfig; print('-L' + sysconfig.get_config_var('LIBDIR') + ' -lpython' + sysconfig.get_config_var('VERSION'))" 2>/dev/null || echo ""`
    AC_SUBST([PYTHON3_CPPFLAGS])
    AC_SUBST([PYTHON3_LDFLAGS])
    AC_SUBST([PYTHON3])
    AC_SUBST([SWIG])
  else
    enable_python=no
  fi
fi

dnl Output
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
