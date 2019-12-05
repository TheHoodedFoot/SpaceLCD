#ifndef BGR_H
#define BGR_H

#include <stdint.h>

void rgbtobgr(uint8_t *buffer, int size);
void swapendianness(uint8_t *bytes);
void addheader(uint8_t *usbdata, int compressed_length, int scroll);

#endif /* end of include guard: BGR_H */
