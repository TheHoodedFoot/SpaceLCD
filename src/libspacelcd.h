#ifndef LIBSPACELCD_H_SLHXDJDE
#define LIBSPACELCD_H_SLHXDJDE

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

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

const uint16_t start01[] = {0x0311, 0x1164};
const uint16_t start02[] = {0x0313, 0x131a};

int svgtolcd(const char *svg, int scroll);
int svgtolcd_partial(const char *svg, int scroll);
int lcd_send_bitmap(const uint16_t *bitmap, int effect);
int lcd_send_bitmap_tiled(const uint16_t *bitmap, int effect, int num_tiles);
int lcd_animate(const uint16_t **bitmaps, int count, int fps, int effect);
/* void setbrightness(int level); */
/* void startup(); */
/* void shutdown(); */
/* void reset(); */

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: LIBSPACELCD_H_SLHXDJDE */
