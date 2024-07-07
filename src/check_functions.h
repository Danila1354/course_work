#ifndef COURSE_WORK_CHECK_FUNCTIONS_H
#define COURSE_WORK_CHECK_FUNCTIONS_H
#include "structs.h"
#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include "parse_functions.h"
int is_inside_polygon(Point polygon[], int n, Point p);
void check_corner_coords(char *left_up, char *right_down);
void check_center_coords(char *coords);
void check_radius(char *radius);
void check_thickness(char *thickness);
void check_color(char *color);
void check_axis(char *axis);
#endif //COURSE_WORK_CHECK_FUNCTIONS_H
