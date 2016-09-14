/*Copyright 2016 Joseph Jimenez
 *This is the implementaion file for palette.h
 */

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "palette.h"
#include "jBitMap.h"

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
      ret->b += 2;
    } else if (i < NUM_COLORS / 2 + NUM_COLORS / 4) {
      ret->r += 2;
    }
    ret->pal[i] = (JColor){ret->r, ret->g, ret->b};     
  }
  return ret;
}

void CreatePaletteImage(Palette *pal, char *name) {
  JBitMap *bmp = AllocateJBitMap(16 * 64, 16 * 64);
  int index = 0;
  for (int i = 0; i < 16 * 64; i += 64) {
    for (int j = 0; j < 16 * 64; j += 64) {
      FillRect(bmp, i, i + 64, j, j + 64, pal->pal[index]);
      index++;
    }
  }
  CreateFile(bmp, name);
  DeleteJBitMap(bmp);
}

void DeletePalette(Palette *pal) {
  free(pal);
}
