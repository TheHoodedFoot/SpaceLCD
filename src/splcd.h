#ifndef SPLCD_H
#define SPLCD_H

#define X_RESOLUTION 640
#define Y_RESOLUTION 150

/* Byte 0x00 */
#define EFFECT_CUT 0x11
#define EFFECT_SCROLL_DOWN 0x14
#define EFFECT_SCROLL_UP 0x15
#define EFFECT_SCROLL_LEFT 0x16
#define EFFECT_SCROLL_RIGHT 0x17
#define EFFECT_PARTIAL 0x18

/* zlib */
#define CHUNK 16384

#define SPLCD_HEADER_SIZE 0x200

const char *whiterect =
	"<svg width=\"640\" height=\"150\" version=\"1.1\"><rect x=\"20\" "
	"y=\"20\" width=\"600\" height=\"110\" ry=\"0\" "
	"fill=\"#008800\"/></svg>";

const char *freecad =
	"<svg version=\"1.1\" viewBox=\"0 0 640 150\" "
	"xmlns=\"http://www.w3.org/2000/svg\" "
	"xmlns:xlink=\"http://www.w3.org/1999/xlink\"><defs><radialGradient "
	"id=\"b\" cx=\"283\" cy=\"29\" r=\"20\" "
	"gradientTransform=\"matrix(.62 .97 -1 .66 -327 -256)\" "
	"gradientUnits=\"userSpaceOnUse\"><stop stop-color=\"#71b2f8\" "
	"offset=\"0\"/><stop stop-color=\"#002795\" "
	"offset=\"1\"/></radialGradient><radialGradient id=\"a\" cx=\"271\" "
	"cy=\"34\" r=\"20\" gradientTransform=\"matrix(1.1 .27 -.75 3.1 -471 "
	"-148)\" gradientUnits=\"userSpaceOnUse\"><stop "
	"stop-color=\"#ff6d0f\" offset=\"0\"/><stop stop-color=\"#ff1000\" "
	"offset=\"1\"/></radialGradient></defs><g transform=\"matrix(2.1 0 0 "
	"2.1 707 -4.3)\" fill-rule=\"evenodd\" stroke-width=\"1.9\"><path "
	"d=\"m-216 7.3v56h12v-22h8.8v-8.6h-8.8v-9.2h22v-16z\" "
	"fill=\"url(#a)\" stroke=\"#370700\"/><path d=\"m-161 24-7.2 "
	"4.1-3.2-1.1-3-7.8-4.8.25-2.2 8-3 1.5-7.6-3.3-3.2 3.6 4.1 7.2-1.1 "
	"3.2-7.8 3 .28 4.8 8 2.2 1.5 3-3.3 7.6 3.6 3.2 7.2-4.1 3.2 1.1 3 7.7 "
	"4.8-.27 2.2-8 3-1.5 7.6 3.4 3.2-3.6-4.1-7.2 1.1-3.2 "
	"7.7-3-.24-4.8-8-2.2-1.5-3 3.3-7.6zm-11 12 1.5.86 1.3 1.2 1 1.4.74 "
	"1.5.43 1.7.072 1.7-.25 1.7-.56 1.6-.89 1.5-1.1 1.3-1.4 "
	"1-1.5.74-1.7.43-1.7.097-1.7-.25-1.6-.56-1.5-.89-1.3-1.2-1-1.4-.74-1."
	"5-.42-1.7-.097-1.7.25-1.7.56-1.6.88-1.5 1.1-1.3 1.4-1 1.5-.74 "
	"1.7-.42 1.7-.072 1.7.25z\" fill=\"url(#b)\" "
	"stroke=\"#000137\"/></g></svg>";

#endif /* end of include guard: SPLCD_H */
