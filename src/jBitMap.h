/* 
 * Copyright 2016 Joseph Jimenez
 * This defines functions and structs for a Bitmap,
 * which creates a gradient of colors to be used
 * on pixelscreates a standard bitmap image
 */

#ifndef _J_BITMAP_H_
#define _J_BITMAP_H_

#include "palette.h"
#include <inttypes.h>

// Struct that represents a BMP image
typedef struct jBMP {
  uint16_t width;
  uint16_t height;
  JColor *pixels;
} JBitMap;

// Returns a JBitMap struct ready to be modified with
// the passed in height and width
JBitMap *AllocateJBitMap(uint16_t width, uint16_t height);

// Frees all memory allocated in the struct
void DeleteJBitMap(JBitMap *bmp);

// Sets the specified pixel to the specified JColor of the bitmap
void SetPixel(JBitMap *bmp, uint16_t x, uint16_t y, JColor col);

// Draws a rectangle from x0 to x1, y0 to y1
void FillRect(JBitMap *bmp, uint16_t x0, uint16_t x1,
	      uint16_t y0, uint16_t y1, JColor col);

// Uses the struct to create the file
void CreateFile(JBitMap *bmp, char *filename);

#endif  // _J_BITMAP_H_
