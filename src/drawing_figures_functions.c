#include "drawing_figures_functions.h"
void draw_circle(PNGImage *image, int x0, int y0, int r, int thickness, int *colors) {
    int x = 0;
    int radius;
    int y = r;
    int delta = 1 - 2 * r;
    int error = 0;
    if (thickness % 2 == 0) {
        radius = thickness / 2;
    } else if (thickness == 1) {
        radius = 0;
    } else {
        radius = (thickness + 1) / 2;
    }
    while (y >= x) {
        fill_circle(image, x0 + x, y0 + y, radius, colors);
        fill_circle(image, x0 + x, y0 - y, radius, colors);
        fill_circle(image, x0 - x, y0 + y, radius, colors);
        fill_circle(image, x0 - x, y0 - y, radius, colors);
        fill_circle(image, x0 + y, y0 + x, radius, colors);
        fill_circle(image, x0 + y, y0 - x, radius, colors);
        fill_circle(image, x0 - y, y0 + x, radius, colors);
        fill_circle(image, x0 - y, y0 - x, radius, colors);

        error = 2 * (delta + y) - 1;
        if ((delta < 0) && (error <= 0)) {
            delta += 2 * ++x + 1;
            continue;
        }
        if ((delta > 0) && (error > 0)) {
            delta -= 2 * --y + 1;
            continue;
        }
        delta += 2 * (++x - --y);
    }
}
void draw_rectangle(PNGImage *image, int x1, int y1, int x2, int y2, int thickness, char *color, int fill,
                    char *fill_color) {
    char *copy_color = strdup(color);

    char *token = strtok(copy_color, ".");
    int colors[3];
    for (int i = 0; i < 3; i++) {
        colors[i] = atoi(token);
        token = strtok(NULL, ".");
    }
    int min_x = x1<x2 ? x1 : x2;
    int max_x = x1<x2 ? x2 : x1;
    int min_y = y1<y2 ? y1 : y2;
    int max_y = y1<y2 ? y2: y1;
    x1 = min_x;
    x2 = max_x;
    y1 = min_y;
    y2 = max_y;
    if (fill) {
        int fill_colors[3];
        char *copy_fill_color = strdup(fill_color);
        token = strtok(copy_fill_color, ".");
        for (int i = 0; i < 3; i++) {
            fill_colors[i] = atoi(token);
            token = strtok(NULL, ".");
        }
        for (int y = y1; y < y2+1; y++) {
            for (int x = x1; x < x2+1; x++) {
                draw_pixel(image, x, y, fill_colors);
            }
        }
        free(copy_fill_color);
    }
    draw_thick_line(image, x1, y1, x2 + 1, y1, thickness, colors );
    draw_thick_line(image, x1, y1, x1, y2 + 1, thickness, colors );
    draw_thick_line(image, x1 + 1, y2, x2 + 1, y2, thickness, colors );
    draw_thick_line(image, x2, y1 + 1, x2, y2 + 1, thickness, colors );
    free(copy_color);
}
void draw_hexagon(PNGImage *image, int center_x, int center_y, int radius, int thickness, char *color, int fill,
                  char *fill_color) {
    // check radius
    if (radius >= 0) {
        char *copy_color = strdup(color);
        char *token = strtok(copy_color, ".");
        int colors[3];
        for (int i = 0; i < 3; i++) {
            colors[i] = atoi(token);
            token = strtok(NULL, ".");
        }
        int x1 = round(center_x - radius/2);
        int y1 = round(center_y-sqrt(radius*radius-(radius/2)*radius/2));
        int x2 = round(center_x + radius/2);
        int y2 = y1;
        int x3 = center_x +radius;
        int y3 = center_y;
        int x4 = x2;
        int y4 = round(center_y+sqrt(radius*radius-(radius/2)*radius/2));
        int x5 = x1;
        int y5 = y4;
        int x6 = center_x-radius;
        int y6 = center_y;
        Point hexagon[] = {{x1, y1},
                           {x2, y2},
                           {x3, y3},
                           {x4, y4},
                           {x5, y5},
                           {x6, y6}};

        if (fill) {
            int fill_colors[3];
            char *copy_fill_color = strdup(fill_color);
            token = strtok(copy_fill_color, ".");
            for (int i = 0; i < 3; i++) {
                fill_colors[i] = atoi(token);
                token = strtok(NULL, ".");
            }
            for (int y = y1 - 1; y < y4 + 1; y++) {
                for (int x = x6; x < x3 + 1; x++) {
                    int flag = 0;
                    Point point = {x, y};
                    if (y >= image->height || x >= image->width || y < 0 || x < 0) {
                        continue;
                    }
                    if (is_inside_polygon(hexagon, 6, point)) {
                        png_byte *ptr = &(image->row_pointers[y][x * 3]);
                        ptr[0] = fill_colors[0];
                        ptr[1] = fill_colors[1];
                        ptr[2] = fill_colors[2];
                    }

                }
            }
            free(copy_fill_color);
        }
        draw_thick_line(image,x1,y1,x2+1,y2,thickness,colors);
        draw_thick_line(image,x2,y2,x3+1,y3+1,thickness,colors);
        draw_thick_line(image,x3,y3,x4-1,y4+1,thickness,colors);
        draw_thick_line(image,x4,y4,x5-1,y5,thickness,colors);
        draw_thick_line(image,x5,y5,x6-1,y6-1,thickness,colors);
        draw_thick_line(image,x6,y6,x1+1,y1-1,thickness,colors);
        free(copy_color);
    }
}
void draw_pentagram(PNGImage *image,int center_x, int center_y,int radius,int thickness,int* colors){
    draw_circle(image,center_x,center_y,radius,thickness,colors);
    int x1 = center_x;
    int y1 = center_y - radius;
    int x2 = round(center_x + radius * sin(0.2 * M_PI));
    int y2 = round(center_y + radius * cos(0.2 * M_PI));

    int x3 = round(center_x - radius * sin(0.2 * M_PI));
    int y3 = round(center_y + radius * cos(0.2 * M_PI));

    int x4 = round(center_x + radius * sin(0.4 * M_PI));
    int y4 = round(center_y - radius * cos(0.4 * M_PI));

    int x5 = round(center_x - radius * sin(0.4 * M_PI));
    int y5 = round(center_y - radius * cos(0.4 * M_PI));

    draw_thick_line(image,x1,y1,x2+1,y2+1,thickness,colors);
    draw_thick_line(image,x5,y5,x2+1,y2+1,thickness,colors);
    draw_thick_line(image,x5,y5,x4+1,y4,thickness,colors);
    draw_thick_line(image,x4,y4,x3-1,y3+1,thickness,colors);
    draw_thick_line(image,x1,y1,x3-1,y3+1,thickness,colors);
}
