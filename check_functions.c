#include "check_functions.h"
int isInsidePolygon(Point polygon[], int n, Point p) {
    int i, j, c = 0;
    for (i = 0, j = n - 1; i < n; j = i++) {
        if (((polygon[i].y > p.y) != (polygon[j].y > p.y)) &&
            (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x))
            c = !c;
    }
    return c;
}
void check_corner_coords(char *left_up, char *right_down) {
    // check with regex that coords are like "x.y"
    regex_t regex;
    int reti = regcomp(&regex, "^[0-9]+\\.[0-9]+$", REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
    reti = regexec(&regex, left_up, 0, NULL, 0);
    if (reti) {
        printf("%s", left_up);
        fprintf(stderr, "Left up coords are not correct\n");
        exit(41);
    }
    reti = regexec(&regex, right_down, 0, NULL, 0);
    if (reti) {
        fprintf(stderr, "Right down coords are not correct\n");
        exit(41);
    }
    int *left_up_coords = parse_coords(left_up);
    int *right_down_coords = parse_coords(right_down);
    if (left_up_coords[0] > right_down_coords[0] || left_up_coords[1] > right_down_coords[1]) {
        char* temp = malloc(sizeof(char)*(strlen(left_up)+1));
        strcpy(temp,left_up);
        strcpy(left_up,right_down);
        strcpy(right_down,temp);
        free(temp);
    }
    free(left_up_coords);
    free(right_down_coords);
}
void check_center_coords(char *coords) {
    regex_t regex;
    int reti = regcomp(&regex, "^[0-9]+\\.[0-9]+$", REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
    reti = regexec(&regex, coords, 0, NULL, 0);
    if (reti) {
        fprintf(stderr, "Coords are not correct\n");
        exit(41);
    }
}
void check_radius(char *radius) {
    regex_t regex;
    int reti = regcomp(&regex, "[0-9]+", REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
    reti = regexec(&regex, radius, 0, NULL, 0);
    if (reti || atoi(radius) < 0) {
        fprintf(stderr, "Radius is not correct\n");
        exit(41);
    }
}
void check_thickness(char *thickness) {
    regex_t regex;
    int reti = regcomp(&regex, "[0-9]+", REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
    reti = regexec(&regex, thickness, 0, NULL, 0);
    if (reti || atoi(thickness) < 0) {
        fprintf(stderr, "Thickness is not correct\n");
        exit(41);
    }
}
void check_color(char *color) {
    regex_t regex;
    // check with regex that color is like "x.y.z" and x,y,z from 0 to 255
    int reti = regcomp(&regex, "[0-9]+\\.[0-9]+\\.[0-9]+", REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
    int *color_rgb = parse_color(color);
    reti = regexec(&regex, color, 0, NULL, 0);
    if (reti || color_rgb[0] > 255 || color_rgb[1] > 255 || color_rgb[2] > 255 || color_rgb[0] < 0 || color_rgb[1] <
                                                                                                      0 ||
        color_rgb[2] < 0) {
        free(color_rgb);
        fprintf(stderr, "Color is not correct\n");
        exit(41);
    }
    free(color_rgb);
}

void check_axis(char *axis) {
    if (strcmp(axis, "x") != 0 && strcmp(axis, "y") != 0) {
        fprintf(stderr, "Axis is not correct\n");
        exit(41);
    }
}

