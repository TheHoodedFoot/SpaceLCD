%module spacelcd
%{
#define SWIG_FILE_WITH_INIT
#include "libspacelcd.h"
%}

%include "libspacelcd.h"

%pythoncode %{
class scroll:
	"""Scroll/effect constants for LCD updates."""
	none, down, up, left, right, partial = { 0x11, 0x14, 0x15, 0x16, 0x17, 0x18 }
%}
