#ifndef COURSE_WORK_STRUCTS_H
#define COURSE_WORK_STRUCTS_H
#include <png.h>
typedef struct Element {
    char *key;
    char *value;
} Element;
typedef struct PNGImage {
    int width, height; // width and height of the image
    png_byte color_type; // color type of the image
    png_byte bit_depth; // bit depth of the image
    png_structp png_ptr; // pointer to the png struct
    png_infop info_ptr; // pointer to the png info struct
    int number_of_passes;
    png_bytep *row_pointers;
} PNGImage;
typedef struct {
    int x, y;
} Point;
#endif //COURSE_WORK_STRUCTS_H
