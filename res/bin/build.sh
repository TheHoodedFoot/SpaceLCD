#!/usr/bin/env bash
set -euo pipefail

# Convenience wrapper: bootstrap autotools, configure, and build.
# For a fresh clone:
#   ./build.sh
# For debug build:
#   ./build.sh -d
# After changes to configure.ac / Makefile.am:
#   ./build.sh

if [ ! -f configure ]; then
	echo "Bootstrapping autotools..."
	autoreconf --force --install
fi

if [[ "${1:-}" = "-d" ]]; then
	echo "Configuring debug build..."
	./configure CFLAGS="-ggdb3 -O0" CXXFLAGS="-ggdb3 -O0" "$@"
else
	echo "Configuring release build..."
	./configure CFLAGS="-O2" CXXFLAGS="-O2" "$@"
fi

make -j"$(nproc 2>/dev/null || echo 1)"

echo ""
echo "Build complete. Run 'make check' for tests, 'make install' to install."
