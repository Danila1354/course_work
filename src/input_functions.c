#include "input_functions.h"
char *find_value(Element *dict, int len, char *key) {
    for (int i = 0; i < len; i++) {
        if (strcmp(key, dict[i].key) == 0) {
            return dict[i].value;
        }
    }
    return "";
}
void check_output_and_input_match(char* input, char* output){
    if (strcmp(input,output)==0){
        fprintf(stderr,"Input and output files are the same\n");
        exit(41);
    }
}
int check_extra_option(Element *dict, int len_dict) {
    int len_main_options = 7;
    char *main_options[] = {"mirror", "pentagram", "rect", "hexagon", "info", "input", "outside_ornament"};
    int count_main_options = 0;
    for (int i = 0; i < len_dict; i++) {
        for (int j = 0; j < len_main_options; j++) {
            if (strcmp(dict[i].key, main_options[j]) == 0) {
                count_main_options++;
            }
        }
    }
    return count_main_options;
}

void check_flags(Element *dict, int len_dict, int *input_flag, int *output_flag, int *info_flag, int *help_flag, int *
mirror_flag, int *axis_flag,
                 int *left_up_flag, int *right_down_flag, int *pentagram_flag, int *center_flag, int *radius_flag, int *
thickness_flag, int *color_flag, int *fill_flag, int *fill_color_flag, int *rect_flag, int *hexagon_flag, int*
outside_ornament_flag) {

    for (int i = 0; i < len_dict; i++) {
        if (strcmp("input", dict[i].key) == 0) {
            *input_flag = 1;
        } else if (strcmp("info", dict[i].key) == 0) {
            *info_flag = 1;
        } else if (strcmp("help", dict[i].key) == 0) {
            *help_flag = 1;
        } else if (strcmp("mirror", dict[i].key) == 0) {
            *mirror_flag = 1;
        } else if (strcmp("axis", dict[i].key) == 0) {
            *axis_flag = 1;
        } else if (strcmp("left_up", dict[i].key) == 0) {
            *left_up_flag = 1;
        } else if (strcmp("right_down", dict[i].key) == 0) {
            *right_down_flag = 1;
        } else if (strcmp("pentagram", dict[i].key) == 0) {
            *pentagram_flag = 1;
        } else if (strcmp("center", dict[i].key) == 0) {
            *center_flag = 1;
        } else if (strcmp("radius", dict[i].key) == 0) {
            *radius_flag = 1;
        } else if (strcmp("thickness", dict[i].key) == 0) {
            *thickness_flag = 1;
        } else if (strcmp("color", dict[i].key) == 0) {
            *color_flag = 1;
        } else if (strcmp("fill", dict[i].key) == 0) {
            *fill_flag = 1;
        } else if (strcmp("fill_color", dict[i].key) == 0) {
            *fill_color_flag = 1;
        } else if (strcmp("rect", dict[i].key) == 0) {
            *rect_flag = 1;
        } else if (strcmp("hexagon", dict[i].key) == 0) {
            *hexagon_flag = 1;
        }
        else if (strcmp("outside_ornament", dict[i].key) == 0) {
            *outside_ornament_flag = 1;
        }
        else if (strcmp("output", dict[i].key) == 0) {
            *output_flag = 1;
        }
    }
}
char *find_main_option(Element *dict, int len_dict, int *input_flag, int *output_flag, int *info_flag, int *help_flag, int *
mirror_flag,int *axis_flag, int *left_up_flag, int *right_down_flag, int *pentagram_flag, int
                       *center_flag, int *radius_flag, int *thickness_flag, int *color_flag, int *fill_flag, int
                       *fill_color_flag, int *rect_flag, int *hexagon_flag, int* outside_ornament_flag) {
    check_flags(dict, len_dict, input_flag, output_flag, info_flag, help_flag, mirror_flag, axis_flag,
                left_up_flag, right_down_flag, pentagram_flag, center_flag, radius_flag,
                thickness_flag, color_flag, fill_flag, fill_color_flag, rect_flag, hexagon_flag,outside_ornament_flag);
    int count_main_options = check_extra_option(dict, len_dict);
    if (count_main_options == 2) {
        if ((*mirror_flag)==1) {
            if ((*axis_flag)==1 && (*left_up_flag==1) && (*right_down_flag)==1 && (*output_flag)==1 && (*input_flag)
                                                                                                       ==1){
                check_corner_coords(find_value(dict, len_dict, "left_up"), find_value(dict, len_dict, "right_down"));
                check_axis(find_value(dict, len_dict, "axis"));
                check_output_and_input_match(find_value(dict, len_dict, "input"), find_value(dict, len_dict, "output"));
                return "mirror";
            } else {
                fprintf(stderr, "Недостаточное количество флагов для функции mirror\n");
                exit(41);
            }
        } else if ((*pentagram_flag)==1) {
            if ((*center_flag)==1 && (*radius_flag)==1 && (*thickness_flag)==1 && (*color_flag)==1 && (*output_flag)
                                                                                                      ==1 && (*input_flag)==1){
                check_center_coords(find_value(dict, len_dict, "center"));
                check_radius(find_value(dict, len_dict, "radius"));
                check_thickness(find_value(dict, len_dict, "thickness"));
                check_color(find_value(dict, len_dict, "color"));
                check_output_and_input_match(find_value(dict, len_dict, "input"), find_value(dict, len_dict, "output"));
                return "pentagram";
            } else {
                fprintf(stderr, "Недостаточное количество флагов для функции pentagram\n");
                exit(41);
            }
        } else if ((*rect_flag)==1) {
            if ((*left_up_flag)==1 && (*right_down_flag)==1 && (*thickness_flag)==1 && (*color_flag)==1 &&
                (*output_flag)==1 && (*input_flag)==1){
                check_corner_coords(find_value(dict, len_dict, "left_up"), find_value(dict, len_dict, "right_down"));
                check_thickness(find_value(dict, len_dict, "thickness"));
                check_color(find_value(dict, len_dict, "color"));
                check_output_and_input_match(find_value(dict, len_dict, "input"), find_value(dict, len_dict, "output"));
                if ((*fill_flag) == 1 && (*fill_color_flag) == 1){
                    check_color(find_value(dict, len_dict, "fill_color"));
                }
                if ((*fill_flag) == 1 && (*fill_color_flag) == 0){
                    fprintf(stderr, "Не задан цвет заливки\n");
                    exit(41);
                }
                return "rect";
            } else {
                fprintf(stderr, "Недостаточное количество флагов для функции rect\n");
                exit(41);
            }
        } else if ((*hexagon_flag)==1) {
            if ((*center_flag)==1 && (*radius_flag)==1 && (*thickness_flag)==1 && (*color_flag)==1 && (*output_flag)
                                                                                                      ==1 && (*input_flag)==1) {
                check_center_coords(find_value(dict, len_dict, "center"));
                check_radius(find_value(dict, len_dict, "radius"));
                check_thickness(find_value(dict, len_dict, "thickness"));
                check_color(find_value(dict, len_dict, "color"));
                check_output_and_input_match(find_value(dict, len_dict, "input"), find_value(dict, len_dict, "output"));
                if ((*fill_flag) == 1 && (*fill_color_flag) == 1){
                    check_color(find_value(dict, len_dict, "fill_color"));
                }
                if ((*fill_flag) == 1 && (*fill_color_flag) == 0){
                    fprintf(stderr, "Не задан цвет заливки\n");
                    exit(41);
                }
                return "hexagon";
            } else {
                fprintf(stderr, "Недостаточное количество флагов для функции hexagon\n");
                exit(41);
            }
        }
        else if ((*outside_ornament_flag)==1) {
            if ((*thickness_flag)==1 && (*color_flag)==1  && (*output_flag)==1 && (*input_flag)==1){
                check_thickness(find_value(dict, len_dict, "thickness"));
                check_color(find_value(dict, len_dict, "color"));
                check_output_and_input_match(find_value(dict, len_dict, "input"), find_value(dict, len_dict, "output"));
                return "outside_ornament";
            } else {
                fprintf(stderr, "Недостаточное количество флагов для функции outside_ornament\n");
                exit(41);
            }
        }
        else if ((*info_flag)==1) {
            return "info";
        }
    } else if (count_main_options > 2) {
        fprintf(stderr, "Можно выполнить только 1 основную функцию, а не несколько\n");
        exit(41);
    }
    return "Not main option";

}


void run(Element *dict, int len_dict) {
    int input_flag = 0;
    int output_flag = 0;
    int info_flag = 0;
    int help_flag = 0;
    int mirror_flag = 0;
    int axis_flag = 0;
    int left_up_flag = 0;
    int right_down_flag = 0;
    int pentagram_flag = 0;
    int center_flag = 0;
    int radius_flag = 0;
    int thickness_flag = 0;
    int color_flag = 0;
    int fill_flag = 0;
    int fill_color_flag = 0;
    int rect_flag = 0;
    int hexagon_flag = 0;
    int outside_ornament_flag = 0;

    char *main_option = find_main_option(dict, len_dict, &input_flag, &output_flag, &info_flag, &help_flag, &mirror_flag,
                                         &axis_flag,
                                         &left_up_flag, &right_down_flag, &pentagram_flag, &center_flag,
                                         &radius_flag, &thickness_flag, &color_flag,
                                         &fill_flag, &fill_color_flag, &rect_flag, &hexagon_flag,
                                         &outside_ornament_flag);

    if (strcmp(main_option, "mirror") == 0) {
        PNGImage image;
        read_png_file(find_value(dict, len_dict, "input"), &image);
        char *axis = find_value(dict, len_dict, "axis");
        char *left_up = find_value(dict, len_dict, "left_up");
        char *right_down = find_value(dict, len_dict, "right_down");
        int *left_up_coords = parse_coords(left_up);
        int *right_down_coords = parse_coords(right_down);
        mirror_image(&image, axis[0], left_up_coords[0], left_up_coords[1], right_down_coords[0],
                     right_down_coords[1]);
        write_png_file(find_value(dict, len_dict, "output"), &image);
        free(left_up_coords);
        free(right_down_coords);
    }
    else if(strcmp(main_option,"pentagram")==0){
        PNGImage image;
        read_png_file(find_value(dict, len_dict, "input"), &image);
        char *center = find_value(dict, len_dict, "center");
        char *radius = find_value(dict, len_dict, "radius");
        char *thickness = find_value(dict, len_dict, "thickness");
        char *color = find_value(dict, len_dict, "color");
        int *center_coords = parse_coords(center);
        int *colors = parse_color(color);
        draw_pentagram(&image,center_coords[0],center_coords[1],atoi(radius),
                       atoi(thickness),colors);
        write_png_file(find_value(dict, len_dict, "output"), &image);
        free(center_coords);
        free(colors);
    }
    else if(strcmp(main_option,"rect")==0){
        PNGImage image;
        read_png_file(find_value(dict, len_dict, "input"), &image);
        char *left_up = find_value(dict, len_dict, "left_up");
        char *right_down = find_value(dict, len_dict, "right_down");
        char *thickness = find_value(dict, len_dict, "thickness");
        char *color = find_value(dict, len_dict, "color");
        char* fill_color = "0.0.0";
        if (fill_flag){
            fill_color = find_value(dict, len_dict, "fill_color");
        }
        int *left_up_coords = parse_coords(left_up);
        int *right_down_coords = parse_coords(right_down);

        draw_rectangle(&image,left_up_coords[0],left_up_coords[1],right_down_coords[0],
                       right_down_coords[1],atoi
                (thickness),color,fill_flag,fill_color);
        write_png_file(find_value(dict, len_dict, "output"), &image);
        free(left_up_coords);
        free(right_down_coords);

    }
    else if(strcmp(main_option,"hexagon")==0){

        PNGImage image;
        read_png_file(find_value(dict, len_dict, "input"), &image);
        char *center = find_value(dict, len_dict, "center");
        char *radius = find_value(dict, len_dict, "radius");
        char *thickness = find_value(dict, len_dict, "thickness");
        char *color = find_value(dict, len_dict, "color");
        int fill = fill_flag == 1 ? 1 : 0;
        char *fill_color = "0.0.0";
        if (fill){
            fill_color = find_value(dict, len_dict, "fill_color");
        }
        int *center_coords = parse_coords(center);
        draw_hexagon(&image,center_coords[0],center_coords[1],atoi(radius),
                     atoi(thickness),color,fill,fill_color);
        write_png_file(find_value(dict, len_dict, "output"), &image);
    }
    else if(strcmp(main_option,"outside_ornament")==0){
        PNGImage image;
        read_png_file(find_value(dict, len_dict, "input"), &image);
        char *thickness = find_value(dict, len_dict, "thickness");
        char *color = find_value(dict, len_dict, "color");
        outside_ornament(&image,atoi(thickness),color);
        write_png_file(find_value(dict, len_dict, "output"), &image);
    }
    else if(strcmp(main_option,"info")==0){
        PNGImage image;
        read_png_file(find_value(dict, len_dict, "input"), &image);
        printf("Width: %d\n",image.width);
        printf("Height: %d\n",image.height);
        printf("Color type: %d\n",image.color_type);
        printf("Bit depth: %d\n",image.bit_depth);
    }
    else{
        if (help_flag){
            printf("Course work for option 5.15, created by Danila Pachev.\n\n"
                   "This program allows you to process PNG images\n"
                   "Available flags\n\n"
                   "--help or -h - shows help information\n"
                   "--info (use with --input) - shows information about image\n"
                   "--input or -i is used to specify the path to the input image. After using this flag, "
                   "you should provide the path to the original image as its argument.\n"
                   "--output or -o is used to specify the output path for the image. After using this flag, "
                   "you need to provide the path for saving the new image as an argument.\n\n"
                   "--mirror - reflects a specified image region across a chosen axis (use with:\n"
                   "  --left_up - takes coords of the top-left corner of an area in the format X.Y\n"
                   "  --right_down - takes coords the right-down corner of an area in the format X.Y\n"
                   "  --axis - takes the axis relative to which you want to reflect)\n"
                   "--rect - draws a rectangle at given coordinates (use with:\n"
                   "  --left_up and --right_down (check desc. in --mirror flag)\n"
                   "  --thickness - takes line thickness\n"
                   "  --color - takes line color in the format R.G.B\n"
                   "  --fill (not necessary) - fill rectangle (use without argument like '--fill'\n"
                   "  --fill_color (not necessary) - if you use flag --fill, you need to specify fill color int the "
                   "format R.G.B)\n"
                   "--pentagram - draws pentagram in the circle (use with:\n"
                   "  --center - takes the coords of the center of the circle into which the pentagram needs to be "
                   "inscribed in the format X.Y\n"
                   "  --radius - takes radius of the circle\n"
                   "  --thickness and --color (check desc. in --rect flag))\n"
                   "--hexagon - draws hexagon (use with:\n"
                   "  --center, --radius, --thickness and --color (check desc. in --pentagram flag))\n"
                   "--outside_ornament - expands the image and draws a frame (use with:\n"
                   "  --thickness and --color (check desc in --rect flag))\n\n"
                   "For example: ./cw --input ./img.png --output ./new_img.png --pentagram --center 100.100 --radius 50"
                   " --thickness 12 --color 255.0.0");
        }
    }
}