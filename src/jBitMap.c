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

void CreateFile(JBitMap *bmp, char *filename) {
  FILE *fp = fopen(filename, "wb+");
  char buf[18];
  buf[0] = 0x42;
  buf[1] = 0x4D;
  buf[2] = 0x4C;
  for (int i = 3; i < 10; i++) {
    buf[i] = 0x00;
  }
  buf[10] = 0x1A;
  for (int i = 11; i < 14; i++) {
    buf[i] = 0x00;
  }
  buf[14] = 0x0C;
  for (int i = 15; i < 18; i++) {
    buf[i] = 0x00;
  }
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
  char buf2[4];
  buf2[0] = 0x01;
  buf2[1] = 0x00;
  buf2[2] = 0x18;
  buf2[3] = 0x00;
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
