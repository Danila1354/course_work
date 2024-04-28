#ifndef COURSE_WORK_PNG_FUNCTIONS_H
#define COURSE_WORK_PNG_FUNCTIONS_H
#include "structs.h"
#include <stdlib.h>
#include <png.h>
void read_png_file(char *file_name, PNGImage *image);
void write_png_file(char *file_name, PNGImage *image);
#endif //COURSE_WORK_PNG_FUNCTIONS_H
