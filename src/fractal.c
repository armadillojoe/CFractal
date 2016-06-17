/*Copyright Joseph Jimenez 2016
 *This program renders a simple fractal
 */

#include <gtk/gtk.h>
#include <cairo.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <inttypes.h>

#include "palette.h"

// Parallelism
#define NUM_THREADS 4

// Max iterations per pixel
#define MAX_ITS 100

// Height and width contants
#define HEIGHT 800
#define WIDTH 1200
   
// Position constants
#define X_PAN 0
#define Y_PAN 160
#define ZOOM 350

// Drawing lock to avoid race conditions
static pthread_mutex_t drawLock;

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
  Palette *p;
  cairo_t *cr;
} Parameter;

// Worker function for threads
void *DrawPixels(void *params);

// Draws a point to the screen
void DrawPoint(cairo_t *cr, guint x, guint y, const gchar *col);

// This functions does the drawing when draw signal recieved
gboolean DrawCallback(GtkWidget *widget, cairo_t *cr, gpointer data);

// Iterate to determine pixel color
void iterate(uint16_t x, uint16_t y, Palette *p, cairo_t *cr);

int main(int argc, char **argv) {
  GtkWidget *window;
  GtkWidget *dArea;
  pthread_mutex_init(&drawLock, NULL);

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  dArea = gtk_drawing_area_new();
  gtk_widget_set_size_request(dArea, WIDTH, HEIGHT);
  g_signal_connect(G_OBJECT(dArea), "draw", G_CALLBACK(DrawCallback), NULL);
  gtk_container_add(GTK_CONTAINER(window), dArea);

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Fractal");

  gtk_widget_show_all(window);

  gtk_main();
  
  return EXIT_SUCCESS;
}

void DrawPoint(cairo_t *cr, guint x, guint y, const gchar *col) {
  cairo_move_to(cr, x - 1, y - 1);
  cairo_line_to(cr, x, y);
  GdkRGBA color;
  
  gdk_rgba_parse(&color, col);
  gdk_cairo_set_source_rgba(cr, &color);

  cairo_stroke(cr);
}

gboolean DrawCallback(GtkWidget *widget, cairo_t *cr, gpointer data) {
  GtkStyleContext *context;
  guint width, height;
  time_t start, finish;

  time(&start);

  context = gtk_widget_get_style_context(widget);
  width = gtk_widget_get_allocated_width(widget);
  height = gtk_widget_get_allocated_height(widget);

  gtk_render_background(context, cr, 0, 0, width, height);

  Palette *palette = AllocatePalette(8, 24, 8);

  Parameter params[NUM_THREADS];
  Point points[WIDTH * HEIGHT];

  int index = 0;
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
    params[i].p = palette;
    params[i].cr = cr;
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

  DeletePalette(palette);

  time(&finish);
  
  double seconds = difftime(finish, start);

  printf("Rendered in about %f seconds.\n", seconds);
  
  return TRUE;
}

void *DrawPixels(void *params) {
  printf("Entered Thread\n");
  Parameter *p = (Parameter *)params;
  for (int i = p->lo; i < p->hi; i++) {
    iterate(p->points[i].x, p->points[i].y, p->p, p->cr);
  }
  printf("Done with Thread\n");
  return NULL;
}

void iterate(uint16_t x, uint16_t y, Palette *p, cairo_t *cr) {
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

  char color[20];
  if (iterations == MAX_ITS) {
    snprintf(color, sizeof(char) * 20, "rgb(0,0,0)");
  } else {
    uint16_t index = (uint16_t)floor(((float)iterations /
				      (float)(MAX_ITS - 1)) * 255);
    GetColorAtIndex(p, index, color);
  }
  pthread_mutex_lock(&drawLock);
  DrawPoint(cr, x, y, (gchar *)color);
  pthread_mutex_unlock(&drawLock);
}
