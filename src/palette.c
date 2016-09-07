/*Copyright 2016 Joseph Jimenez
 *This is the implementaion file for palette.h
 */

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "palette.h"

void GetColorAtIndex(Palette *pal, int index, JColor *ret) {
  JColor col = pal->pal[index];
  *ret = col;
}

Palette *AllocatePalette(uint8_t r, uint8_t g, uint8_t b) {
  Palette *ret = (Palette *)malloc(sizeof(Palette));
  if (ret == NULL) {
    exit(EXIT_FAILURE);
  }
  ret->r = r;
  ret->g = g;
  ret->b = b;
  for (int i = 0; i < NUM_COLORS; i++) {
    if (i < NUM_COLORS / 4) {
      ret->g += 3;
    } else if (i < NUM_COLORS / 2) {
      ret->b += 3;
    } else if (i < NUM_COLORS / 2 + NUM_COLORS / 4) {
      ret->r += 3;
    }
    ret->pal[i] = (JColor){ret->r, ret->g, ret->b};     
  }
  return ret;
}

void DeletePalette(Palette *pal) {
  free(pal);
}
