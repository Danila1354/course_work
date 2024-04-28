#ifndef COURSE_WORK_DRAWING_LINE_FUNCTIONS_H
#define COURSE_WORK_DRAWING_LINE_FUNCTIONS_H
#include <png.h>
#include "structs.h"
#include <stdlib.h>
void draw_pixel(PNGImage *image, int x, int y, int *colors);
void fill_circle(PNGImage *image, int x0, int y0, int r, int *colors);
void draw_thick_line(PNGImage *image, int x1, int y1, int x2, int y2, int thickness, int *colors);
#endif //COURSE_WORK_DRAWING_LINE_FUNCTIONS_H
