#include "drawing_line_functions.h"


void draw_pixel(PNGImage *image, int x, int y, int *colors) {
    if (!(x < 0 || y < 0 || x >= image->width || y >= image->height)) {
        image->row_pointers[y][x * 3] = colors[0];
        image->row_pointers[y][x * 3 + 1] = colors[1];
        image->row_pointers[y][x * 3 + 2] = colors[2];
    }
}
void fill_circle(PNGImage *image, int x0, int y0, int r, int *colors) {
    int x = 0;
    int y = r;
    int delta = 3 - 2 * y;
    while (y >= x) {
        draw_pixel(image, x0 + x, y0 + y, colors);
        draw_pixel(image, x0 + x, y0 - y, colors);
        draw_pixel(image, x0 - x, y0 + y, colors);
        draw_pixel(image, x0 - x, y0 - y, colors);
        draw_pixel(image, x0 + y, y0 + x, colors);
        draw_pixel(image, x0 + y, y0 - x, colors);
        draw_pixel(image, x0 - y, y0 + x, colors);
        draw_pixel(image, x0 - y, y0 - x, colors);
        delta += delta < 0 ? 4 * x + 6 : 4 * (x - y--) + 10;
        ++x;
    }
    for (int y = -r; y <= r; y++) {
        if ((y0+y)<0 || (y0+y)>=image->height){
            continue;
        }
        for (int x = -r; x <= r; x++) {
            if (((x0+x)>=0) && ((x0+x)<image->width)&& (x * x + y * y <= r * r)) {
                draw_pixel(image, x0 + x, y0 + y, colors);
            }

        }
    }
}
void draw_thick_line(PNGImage *image, int x1, int y1, int x2, int y2, int thickness, int *colors) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    int e2;
    int x = x1;
    int y = y1;
    while (x != x2 || y != y2) {
        draw_pixel(image,x,y,colors);
        if (thickness % 2 == 0) {
            fill_circle(image, x, y, thickness / 2, colors);
        } else if (thickness == 1) {
            fill_circle(image, x, y, 0, colors);
        } else {
            fill_circle(image, x, y, (thickness + 1) / 2, colors);
        }
        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}