#ifndef COURSE_WORK_DRAWING_FIGURES_FUNCTIONS_H
#define COURSE_WORK_DRAWING_FIGURES_FUNCTIONS_H
#include <math.h>
#include <string.h>
#include "structs.h"
#include "drawing_line_functions.h"
#include "check_functions.h"

void draw_circle(PNGImage *image, int x0, int y0, int r, int thickness, int *colors);
void draw_rectangle(PNGImage *image, int x1, int y1, int x2, int y2, int thickness, char *color, int fill,
                    char *fill_color);
void draw_hexagon(PNGImage *image, int center_x, int center_y, int radius, int thickness, char *color, int fill,
                  char *fill_color);
void draw_pentagram(PNGImage *image,int center_x, int center_y,int radius,int thickness,int* colors);
#endif //COURSE_WORK_DRAWING_FIGURES_FUNCTIONS_H
