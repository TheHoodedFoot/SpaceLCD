#ifndef T_SVG_HPP
#define T_SVG_HPP

#ifdef __cplusplus
extern "C" {
#endif

#define BITMAP_WIDTH  640
#define BITMAP_HEIGHT 150
#define BITMAP_BPP    2
#define BITMAP_BYTES  BITMAP_WIDTH*BITMAP_HEIGHT*BITMAP_BPP
	
int svgtorgb565(const char *svg, const int size, char *buffer);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: T_SVG_HPP */
