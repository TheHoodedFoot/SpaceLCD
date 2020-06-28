AC_INIT([spacelcd], [2020.2.29], [thf@thehoodedfoot.com])

AM_PROG_AR()
LT_INIT
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CXX([g++ clang++])
AC_PROG_CC([gcc clang])
PKG_CHECK_MODULES([ZLIB], [zlib])
PKG_CHECK_MODULES([LIBRSVG], [librsvg-2.0])
PKG_CHECK_MODULES([LIBUSB], [libusb-1.0])
AC_CONFIG_MACRO_DIRS([m4])

# This must be after any environment variables are set,
# such as by PKG_CHECK_MODULES
AC_OUTPUT(Makefile)
