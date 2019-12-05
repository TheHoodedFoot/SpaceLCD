#ifndef ZPIPE_H
#define ZPIPE_H

#include <stdint.h>

#define DEFLATED_MAX 0xffff

int crush(uint8_t *source, uint8_t *dest, int len);

#endif /* end of include guard: ZPIPE_H */
