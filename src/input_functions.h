#ifndef COURSE_WORK_INPUT_FUNCTIONS_H
#define COURSE_WORK_INPUT_FUNCTIONS_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
#include "png_functions.h"
#include "drawing_figures_functions.h"
#include "parse_functions.h"
#include "mirror_function.h"
#include "outside_ornament.h"
char *find_value(Element *dict, int len, char *key);
void check_output_and_input_match(char* input, char* output);
int check_extra_option(Element *dict, int len_dict);
void check_flags(Element *dict, int len_dict, int *input_flag, int *output_flag, int *info_flag, int *help_flag, int *
        mirror_flag, int *axis_flag,int *left_up_flag, int *right_down_flag, int *pentagram_flag, int *center_flag,
        int *radius_flag,int *thickness_flag, int *color_flag, int *fill_flag, int *fill_color_flag, int *rect_flag,
        int *hexagon_flag,int* outside_ornament_flag);

char *find_main_option(Element *dict, int len_dict, int *input_flag, int *output_flag, int *info_flag, int *help_flag, int *
mirror_flag,int *axis_flag, int *left_up_flag, int *right_down_flag, int *pentagram_flag, int
                       *center_flag, int *radius_flag, int *thickness_flag, int *color_flag, int *fill_flag, int
                       *fill_color_flag, int *rect_flag, int *hexagon_flag,int* outside_ornament_flag);
void run(Element *dict, int len_dict);
#endif //COURSE_WORK_INPUT_FUNCTIONS_H
