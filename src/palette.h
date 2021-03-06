/*
 * Copyright 2016 Joseph Jimenez
 * This defines functions and structs for a Palette,
 * which creates a gradient of colors to be used
 * on pixels
 */

#ifndef _PALETTE_H_
#define _PALETTE_H_

#include <inttypes.h>

#include "jColor.h"

#define NUM_COLORS 256

// A struct to represent a Palette, keeps track of all the colors
// and a current color
typedef struct palette {
  JColor pal[NUM_COLORS];
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Palette;

// Takes a Palette pointer, an index, and a 20 byte char array, and
// returns a string representation through char *ret of the color
// of the Palette at the index. By default index is 0-255
void GetColorAtIndex(Palette *pal, int index, JColor *ret);

// Creates a new Palette with initial r, g, b values. Caller responsible
// for freeing it with DeletePalette
Palette *AllocatePalette(uint8_t r, uint8_t g, uint8_t b);

// Free memory used by the Palette
void DeletePalette(Palette *pal);

// Makes an image of the palette
void CreatePaletteImage(Palette *pal, char *name);

#endif  // _PALETTE_H_
