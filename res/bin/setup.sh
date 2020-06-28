#!/usr/bin/env bash

autoreconf --force --install

if [[ "$1" = "-d" ]]
then
	echo "Enabling debug build"
	./configure CFLAGS="-ggdb3 -O0" CXXFLAGS="${CFLAGS}"
else
	echo "Enabling release build"
	./configure CFLAGS="-O2" CXXFLAGS="${CFLAGS}"
fi

make -j$(nproc)

exit 0



Using Autotools
---------------

Create configure.ac

Run:
	(possibly?) libtoolize
	aclocal

which generates aclocal.m4

Run:
	autoconf

which creates configure

Create Makefile.am

Run:
	automake --add-missing --foreign

Now we can run ./configure and make



Automating the above
--------------------

Run:
	autoreconf --force --install

to run the above scripts in the right order



Using pkg-config
----------------

Add the following to configure.ac:

	PKG_CHECK_MODULES(<NAME>, <PKG-CONFIG-NAME>)

We can get the PKG-CONFIG-NAME using:

	pkg-config --list-all

Then add the following lines to Makefile.am:

	<PROGRAM>_CFLAGS = $(<NAME>_CFLAGS)
	<PROGRAM>_LDFLAGS = $(<NAME>_LIBS)
