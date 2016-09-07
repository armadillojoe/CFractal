/*Copyright 2016 Joseph Jimenez
 *This program renders a simple fractal
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <inttypes.h>

#include "palette.h"
#include "jColor.h"
#include "jBitMap.h"

// Parallelism
#define NUM_THREADS 64

// Max iterations per pixel
#define MAX_ITS 100

// Height and width contants
#define HEIGHT 1400
#define WIDTH 1400
   
// Position constants
#define X_PAN 0
#define Y_PAN 210
#define ZOOM 600

// Stores all calculated color indexes for the pixels
static int colorIndexes[HEIGHT * WIDTH];

// Simple Point struct where x and y represent pixels
typedef struct point {
  uint16_t x;
  uint16_t y;
} Point;

// Parameter struct for threads, lo and hi are points in array points
typedef struct parameters {
  int lo;
  int hi;
  Point *points;
} Parameter;

// Worker function for threads
void *DrawPixels(void *params);

// This functions does the drawing when draw signal recieved
void Render(void);

// Iterate to determine pixel color
void iterate(uint16_t x, uint16_t y, int index);

int main(int argc, char **argv) {
  Render();
  
  return EXIT_SUCCESS;
}

void Render(void) {
  JBitMap *bmp = AllocateJBitMap(WIDTH, HEIGHT);
  time_t start, finish;

  time(&start);

  Palette *palette = AllocatePalette(8, 24, 8);

  Parameter params[NUM_THREADS];
  Point points[WIDTH * HEIGHT];

  int index = 0;
  printf("Drawing\n");
  for (uint16_t y = 0; y < HEIGHT; y++) {
    for (uint16_t x = 0; x < WIDTH; x++) {
      points[index] = (Point){x, y};
      index++;
    }
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    int scaleFactor = WIDTH * HEIGHT / NUM_THREADS;
    params[i].lo = i * scaleFactor;
    params[i].hi = (i + 1) * scaleFactor;
    params[i].points = points;
  }

  pthread_t threads[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; i++) {
    if (pthread_create(&threads[i], NULL, &DrawPixels, &params[i]) != 0) {
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      exit(EXIT_FAILURE);
    }
  }

  index = 0;
  for (uint16_t y = 0; y < HEIGHT; y++) {
    for (uint16_t x = 0; x < WIDTH; x++) {
      if (colorIndexes[index] == -1) {
	SetPixel(bmp, x, y, (JColor){0x00, 0x00, 0x00});
      } else {
	JColor col;
	GetColorAtIndex(palette, colorIndexes[index], &col);
	SetPixel(bmp, x, y, col);
      }
      index++;
    }
  }

  CreateFile(bmp, "fractal.bmp");

  DeleteJBitMap(bmp);

  DeletePalette(palette);

  time(&finish);
  
  double seconds = difftime(finish, start);

  printf("Rendered in about %f seconds.\n", seconds);
}

void *DrawPixels(void *params) {
  Parameter *p = (Parameter *)params;
  for (int i = p->lo; i < p->hi; i++) {
    iterate(p->points[i].x, p->points[i].y, i);
  }
  return NULL;
}

void iterate(uint16_t x, uint16_t y, int index) {
  float x0 = (float)(x - (WIDTH / 2) - X_PAN) / ZOOM;
  float y0 = (float)(y - (HEIGHT / 2) - Y_PAN) / ZOOM;

  float a = 0;
  float b = 0;
  float rx = 0;
  float ry = 0;

  int iterations = 0;
  while (iterations < MAX_ITS && (rx * rx + ry * ry <= 4)) {
    ry = a * a - b * b + y0;
    rx = 2 * a * b + x0;

    b = rx;
    a = ry;
    iterations++;
  }

  if (iterations == MAX_ITS) {
    colorIndexes[index] = -1;
  } else {
    uint16_t ind = (uint16_t)floor(((float)iterations /
				      (float)(MAX_ITS - 1)) * 255);
    colorIndexes[index] = ind;
  }
}
