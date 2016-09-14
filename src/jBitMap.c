/*
 * Copyright 2016 Joseph Jimenez
 * Implementation file for jBitMap.h
 */

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "jBitMap.h"
#include "jColor.h"

JBitMap *AllocateJBitMap(uint16_t width, uint16_t height) {
  JBitMap *ret = (JBitMap *)malloc(sizeof(JBitMap));
  if (ret == NULL) {
    exit(EXIT_FAILURE);
  }
  ret->width = width;
  ret->height = height;
  ret->pixels = (JColor *)malloc(sizeof(JColor) * width * height);
  if (ret->pixels == NULL) {
    exit(EXIT_FAILURE);
  }
  return ret;
}

void DeleteJBitMap(JBitMap *bmp) {
  free(bmp->pixels);
  free(bmp);
}

void SetPixel(JBitMap *bmp, uint16_t x, uint16_t y, JColor col) {
  int yFlip = bmp->height - y - 1;
  int index = (bmp->width * yFlip) + x;
  bmp->pixels[index] = col;
}

void FillRect(JBitMap *bmp, uint16_t x0, uint16_t x1,
	      uint16_t y0, uint16_t y1, JColor col) {
  for (int i = x0; i < x1; i++) {
    for (int j = y0; j < y1; j++) {
      SetPixel(bmp, i, j, col);
    }
  }
}

void CreateFile(JBitMap *bmp, char *filename) {
  FILE *fp = fopen(filename, "wb+");
  char buf[18] = {0x42, 0x4D, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x1A, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00};
  if (fp == NULL) {
    exit(EXIT_FAILURE);
  }
  size_t ret = fwrite(buf, 1, 18, fp);
  if (ret != 18) {
    exit(EXIT_FAILURE);
  }
  ret = fwrite(&(bmp->width), 2, 1, fp);
  if (ret != 1) {
    exit(EXIT_FAILURE);
  }
  ret = fwrite(&(bmp->height), 2, 1, fp);
  if (ret != 1) {
    exit(EXIT_FAILURE);
  }
  char buf2[4] = {0x01, 0x00, 0x18, 0x00};
  ret = fwrite(buf2, 1, 4, fp);
  if (ret != 4) {
    exit(EXIT_FAILURE);
  }
  int numPixels = bmp->height * bmp->width;
  for (int i = 0; i < numPixels; i++) {
    ret = fwrite(&(bmp->pixels[i].b), 1, 1, fp);
    if (ret != 1) {
      exit(EXIT_FAILURE);
    }
    ret = fwrite(&(bmp->pixels[i].g), 1, 1, fp);
    if (ret != 1) {
      exit(EXIT_FAILURE);
    }
    ret = fwrite(&(bmp->pixels[i].r), 1, 1, fp);
    if (ret != 1) {
      exit(EXIT_FAILURE);
    }
  }
  char end = 0x00;
  ret = fwrite(&end, 1, 1, fp);
  if (ret != 1) {
    exit(EXIT_FAILURE);
  }
  fclose(fp);
}
