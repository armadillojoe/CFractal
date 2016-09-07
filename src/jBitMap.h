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

#define HEADER 0x424D
#define SIZE 0x4C000000
#define RESERVED 0x00000000
#define OFFSET 0x1A000000
#define INFOHEADER 0x0C000000
#define CLOSE 0x01001800

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

// Uses the struct to create the file
void CreateFile(JBitMap *bmp, char *filename);

#endif  // _J_BITMAP_H_
