#include <stdio.h>
#include </opt/homebrew/opt/libpng/include/png.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <getopt.h>

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

int isInsidePolygon(Point polygon[], int n, Point p) {
    int i, j, c = 0;
    for (i = 0, j = n - 1; i < n; j = i++) {
        if (((polygon[i].y > p.y) != (polygon[j].y > p.y)) &&
            (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x))
            c = !c;
    }
    return c;
}


void read_png_file(char *file_name, PNGImage *image) {
    int x, y;
    char header[8]; // 8 is the maximum size that can be checked

    FILE *fp = fopen(file_name, "rb");
    if (!fp) {
        // Some error handling: file could not be opened
    }

    fread(header, 1, 8, fp);
    if (png_sig_cmp((png_const_bytep) (header), 0, 8)) {
        // Some error handling: file is not recognized as a PNG
    }

    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!image->png_ptr) {
        // Some error handling: png_create_read_struct failed
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr) {
        png_destroy_read_struct(&image->png_ptr, NULL, NULL);
        exit(0);
    }

    if (setjmp(png_jmpbuf(image->png_ptr))) {
        // Some error handling: error during init_io
    }

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8);

    png_read_info(image->png_ptr, image->info_ptr);

    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);

    image->number_of_passes = png_set_interlace_handling(image->png_ptr);
    png_read_update_info(image->png_ptr, image->info_ptr);

    /* read file */
    if (setjmp(png_jmpbuf(image->png_ptr))) {
        // Some error handling: error during read_image
    }

    image->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * image->height);
    for (y = 0; y < image->height; y++)
        image->row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));

    png_read_image(image->png_ptr, image->row_pointers);

}

//
void write_png_file(char *file_name, PNGImage *image) {
    FILE *fp = fopen(file_name, "wb");
    if (!fp) {
        // Some error handling: file could not be opened
    }
    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!image->png_ptr) {
        // Some error handling: png_create_write_struct failed
    }
    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr) {
        // Some error handling: png_create_info_struct failed
    }
    png_init_io(image->png_ptr, fp);
    if (setjmp(png_jmpbuf(image->png_ptr))) {
        // Some error handling: error during writing header
    }
    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height, image->bit_depth, image->color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
    png_write_info(image->png_ptr, image->info_ptr);
    if (setjmp(png_jmpbuf(image->png_ptr))) {
        // Some error handling: error during writing bytes
    }
    png_write_image(image->png_ptr, image->row_pointers);
    if (setjmp(png_jmpbuf(image->png_ptr))) {
        // Some error handling: error during end of write
    }
    png_write_end(image->png_ptr, NULL);
    for (int y = 0; y < image->height; y++) {
        free(image->row_pointers[y]);
    }
    free(image->row_pointers);
    fclose(fp);
}

int check_coords(int x1, int y1, int x2, int y2, int width, int height) {
    if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || x1 > x2 || y1 > y2 || (x1 > width) || (y1 > height)) {
        return 0;
    }
    return 1;
}


void drawpixel(PNGImage *image, int x, int y, int *colors) {
    if (!(x < 0 || y < 0 || x >= image->width || y >= image->height)) {
        image->row_pointers[y][x * 3] = colors[0];
        image->row_pointers[y][x * 3 + 1] = colors[1];
        image->row_pointers[y][x * 3 + 2] = colors[2];
    }
}

void drawSquare(PNGImage *image, int x0, int y0, int radius, int *colors) {
    if (radius % 2 == 0) {
        radius = radius / 2;
    } else {
        radius = (radius + 1) / 2;
    }
    int x1 = x0 - radius;
    int x2 = x0 + radius;
    int y1 = y0 - radius;
    int y2 = y0 + radius;
    for (int x = x1; x < x2 + 1; x++) {
        for (int y = y1; y < y2 + 1; y++) {
            drawpixel(image, x, y, colors);
        }
    }
}

void fill_circle(PNGImage *image, int x0, int y0, int r, int *colors, int *fill_x_array,
                 int *fill_y_array, int *len_x, int *len_y) {
    int x = 0;
    int y = r;
    int delta = 3 - 2 * y;
    int error = 0;
    while (y >= x) {
        drawpixel(image, x0 + x, y0 + y, colors);
        drawpixel(image, x0 + x, y0 - y, colors);
        drawpixel(image, x0 - x, y0 + y, colors);
        drawpixel(image, x0 - x, y0 - y, colors);
        drawpixel(image, x0 + y, y0 + x, colors);
        drawpixel(image, x0 + y, y0 - x, colors);
        drawpixel(image, x0 - y, y0 + x, colors);
        drawpixel(image, x0 - y, y0 - x, colors);
        if (fill_x_array != NULL && fill_y_array != NULL) {
            fill_x_array[(*len_x)++] = x0 + x;
            fill_y_array[(*len_y)++] = y0 + y;
            fill_x_array[(*len_x)++] = x0 + x;
            fill_y_array[(*len_y)++] = y0 - y;
            fill_x_array[(*len_x)++] = x0 - x;
            fill_y_array[(*len_y)++] = y0 + y;
            fill_x_array[(*len_x)++] = x0 - x;
            fill_y_array[(*len_y)++] = y0 - y;
            fill_x_array[(*len_x)++] = x0 + y;
            fill_y_array[(*len_y)++] = y0 + x;
            fill_x_array[(*len_x)++] = x0 + y;
            fill_y_array[(*len_y)++] = y0 - x;
            fill_x_array[(*len_x)++] = x0 - y;
            fill_y_array[(*len_y)++] = y0 + x;
            fill_x_array[(*len_x)++] = x0 - y;
            fill_y_array[(*len_y)++] = y0 - x;

        }
        delta += delta < 0 ? 4 * x + 6 : 4 * (x - y--) + 10;
        ++x;
    }
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x * x + y * y <= r * r) {
                drawpixel(image, x0 + x, y0 + y, colors);
            }
        }
    }
}


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
        fill_circle(image, x0 + x, y0 + y, radius, colors, NULL, NULL, 0, 0);
        fill_circle(image, x0 + x, y0 - y, radius, colors, NULL, NULL, 0, 0);
        fill_circle(image, x0 - x, y0 + y, radius, colors, NULL, NULL, 0, 0);
        fill_circle(image, x0 - x, y0 - y, radius, colors, NULL, NULL, 0, 0);
        fill_circle(image, x0 + y, y0 + x, radius, colors, NULL, NULL, 0, 0);
        fill_circle(image, x0 + y, y0 - x, radius, colors, NULL, NULL, 0, 0);
        fill_circle(image, x0 - y, y0 + x, radius, colors, NULL, NULL, 0, 0);
        fill_circle(image, x0 - y, y0 - x, radius, colors, NULL, NULL, 0, 0);
//        drawSquare(image, x0 + x, y0 + y, radius, colors);
//        drawSquare(image, x0 + x, y0 - y, radius, colors);
//        drawSquare(image, x0 - x, y0 + y, radius, colors);
//        drawSquare(image, x0 - x, y0 - y, radius, colors);
//        drawSquare(image, x0 + y, y0 + x, radius, colors);
//        drawSquare(image, x0 + y, y0 - x, radius, colors);
//        drawSquare(image, x0 - y, y0 + x, radius, colors);
//        drawSquare(image, x0 - y, y0 - x, radius, colors);
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

void drawThickLine(PNGImage *image, int x1, int y1, int x2, int y2, int thickness, int *colors, int *fill_x_array,
                   int *fill_y_array, int *len_x, int *len_y) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    int e2;
    int x = x1;
    int y = y1;

    while (x != x2 || y != y2) {
        // set pixel

        drawpixel(image,x,y,colors);
        if (thickness % 2 == 0) {
            fill_circle(image, x, y, thickness / 2, colors, fill_x_array, fill_y_array, len_x, len_y);
        } else if (thickness == 1) {
            fill_circle(image, x, y, 0, colors, fill_x_array, fill_y_array, len_x, len_y);
        } else {
            fill_circle(image, x, y, (thickness + 1) / 2, colors, fill_x_array, fill_y_array, len_x, len_y);
        }


        e2 = 90 * err;
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
void draw_line_square(PNGImage *image, int x1, int y1, int x2, int y2, int thickness, int *colors){
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    int e2;
    int x = x1;
    int y = y1;

    while (x != x2 || y != y2) {
        // set pixel

        png_byte *ptr = &(image->row_pointers[y][x * 3]);
        ptr[0] = colors[0];
        ptr[1] = colors[1];
        ptr[2] = colors[2];

        drawSquare(image, x, y, thickness, colors);

        e2 = 90 * err;
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


//void mirror_image(PNGImage *image, char axis, int x1, int y1, int x2, int y2) {
//    if (check_coords(x1, y1, x2, y2,image->width,image->height)) {
//        if (axis == 'x') {
//            if (y2>image->height){
//                y2 = image->height-1;
//            }
//            if (x2>image->width){
//                x2 = image->width-1;
//            }
//            for (int y = y1; y < y2; y++) {
//                png_byte *row = image->row_pointers[y];
//
//                for (int x = 0; x < (x2) / 2; x++) {
//                    png_byte *ptr1 = &(row[x * 3]);
//
//                    int color1 = ptr1[0];
//                    int color2 = ptr1[1];
//                    int color3 = ptr1[2];
//
//                    png_byte *ptr2 = &(row[(x2 - x) * 3]);
//                    ptr1[0] = ptr2[0];
//                    ptr1[1] = ptr2[1];
//                    ptr1[2] = ptr2[2];
//                    ptr2[0] = color1;
//                    ptr2[1] = color2;
//                    ptr2[2] = color3;
//                }
//            }
//        } else if (axis == 'y') {
//            if (y2>image->height){
//                y2 = image->height-1;
//            }
//            if (x2>image->width){
//                x2 = image->width-1;
//            }
//            for (int y = y1; y < y2 / 2; y++) {
//                png_byte *row = image->row_pointers[y];
//                png_byte *row2 = image->row_pointers[y2 - y - 1];
//                for (int x = 0; x < x2; x++) {
//                    png_byte *ptr1 = &(row[x * 3]);
//                    int color1 = ptr1[0];
//                    int color2 = ptr1[1];
//                    int color3 = ptr1[2];
//                    png_byte *ptr2 = &(row2[x * 3]);
//                    ptr1[0] = ptr2[0];
//                    ptr1[1] = ptr2[1];
//                    ptr1[2] = ptr2[2];
//                    ptr2[0] = color1;
//                    ptr2[1] = color2;
//                    ptr2[2] = color3;
//                }
//            }
//        }
//    }
//}
//
void draw_rectangle(PNGImage *image, int x1, int y1, int x2, int y2, int thickness, char *color, int fill,
                    char *fill_color) {
    // chekc coords
    char *copy_color = strdup(color);

    char *token = strtok(copy_color, ".");
    int colors[3];
    for (int i = 0; i < 3; i++) {
        colors[i] = atoi(token);
        token = strtok(NULL, ".");
    }
    int fill_colors[3];
    char *copy_fill_color = strdup(fill_color);
    token = strtok(copy_fill_color, ".");
    for (int i = 0; i < 3; i++) {
        fill_colors[i] = atoi(token);
        token = strtok(NULL, ".");
    }

    if (thickness > 0) {
        drawSquare(image, x1, y1, thickness, colors);
        drawThickLine(image, x1, y1, x2 + 1, y1, thickness, colors, NULL, NULL, 0, 0);
        drawSquare(image, x2, y1, thickness, colors);
        drawThickLine(image, x1, y1, x1, y2 + 1, thickness, colors, NULL, NULL, 0, 0);
        drawSquare(image, x1, y2, thickness, colors);
        drawThickLine(image, x1 + 1, y2, x2 + 1, y2, thickness, colors, NULL, NULL, 0, 0);
        drawSquare(image, x2, y2, thickness, colors);
        drawThickLine(image, x2, y1 + 1, x2, y2 + 1, thickness, colors, NULL, NULL, 0, 0);
    }
    if (fill) {
        int offset;
        if (thickness % 2 == 0) {
            offset = thickness / 2;
        } else {
            offset = (thickness - 1) / 2;
        }
        for (int y = y1 + offset + 1; y < y2 - offset; y++) {
            for (int x = x1 + offset + 1; x < x2 - offset; x++) {
                drawpixel(image, x, y, fill_colors);
            }
        }
    }
    free(copy_color);
    free(copy_fill_color);

}
//

//
//
void draw_hexagon(PNGImage *image, int center_x, int center_y, int radius, int thickness, char *color, int fill,
                  char *fill_color, int iteration) {
    // check radius and thickness
    if (radius > 0) {
        char *copy_color = strdup(color);
        char *token = strtok(copy_color, ".");
        int colors[3];
        for (int i = 0; i < 3; i++) {
            colors[i] = atoi(token);
            token = strtok(NULL, ".");
        }
        int fill_colors[3];
        char *copy_fill_color = strdup(fill_color);
        token = strtok(copy_fill_color, ".");
        for (int i = 0; i < 3; i++) {
            fill_colors[i] = atoi(token);
            token = strtok(NULL, ".");
        }
        // remake to dynamic
        int x_coords_array[10024];
        int y_coords_array[10024];
        int x_array_len = 0;
        int y_array_len = 0;

        int x1 = round(center_x - (sqrt(radius * radius - (radius / 2) * (radius / 2))));
        int y1 = round(center_y - (radius / 2));
        int x2 = x1;
        int y2 = round(center_y + (radius / 2));
        int x3 = center_x;
        int y3 = center_y + radius;
        int x4 = round(center_x + (sqrt(radius * radius - (radius / 2) * (radius / 2))));
        int y4 = round(center_y + (radius / 2));
        int x5 = x4;
        int y5 = round(center_y - (radius / 2));
        int x6 = center_x;
        int y6 = center_y - radius;
        Point hexagon[] = {{x1, y1},
                           {x2, y2},
                           {x3, y3},
                           {x4, y4},
                           {x5, y5},
                           {x6, y6}};
        drawThickLine(image, x1, y1, x2, y2 + 1, thickness, colors, x_coords_array, y_coords_array, &x_array_len,
                      &y_array_len);
        drawThickLine(image, x2, y2, x3 + 1, y3 + 1, thickness, colors, x_coords_array, y_coords_array, &x_array_len,
                      &y_array_len);
        drawThickLine(image, x3, y3, x4 + 1, y4 - 1, thickness, colors, x_coords_array, y_coords_array, &x_array_len,
                      &y_array_len);
        drawThickLine(image, x4, y4, x5, y5 - 1, thickness, colors, x_coords_array, y_coords_array, &x_array_len,
                      &y_array_len);
        drawThickLine(image, x5, y5, x6 - 1, y6 - 1, thickness, colors, x_coords_array, y_coords_array, &x_array_len,
                      &y_array_len);
        drawThickLine(image, x6, y6, x1 - 1, y1 + 1, thickness, colors, x_coords_array, y_coords_array, &x_array_len,
                      &y_array_len);
        if (fill) {
            for (int y = y6 - 1; y < y3 + 1; y++) {
                for (int x = x1; x < x4 + 1; x++) {
                    int flag = 0;
                    Point point = {x, y};
                    for (int i = 0; i < x_array_len; i++) {
                        if (x == x_coords_array[i] && y == y_coords_array[i]) {
                            flag = 1;
                            break;
                        }
                    }
                    if (y >= image->height || x >= image->width || y < 0 || x < 0) {
                        continue;
                    }
                    if (flag == 0 && isInsidePolygon(hexagon, 6, point)) {
                        png_byte *ptr = &(image->row_pointers[y][x * 3]);
                        ptr[0] = fill_colors[0];
                        ptr[1] = fill_colors[1];
                        ptr[2] = fill_colors[2];
                    }

                }
            }
        }
    }
}
void pentagram(PNGImage *image,int center_x, int center_y,int radius,int thickness,int* colors){
    draw_circle(image,center_x,center_y,radius,thickness,colors);
    int x1 = center_x;
    int y1 = center_y - radius;
    int x2 = center_x + radius * sin(0.2 * M_PI);
    int y2 = center_y + radius * cos(0.2 * M_PI);

    int x3 = center_x - radius * sin(0.2 * M_PI);
    int y3 = center_y + radius * cos(0.2 * M_PI);

    int x4 = center_x + radius * sin(0.4 * M_PI);
    int y4 = center_y - radius * cos(0.4 * M_PI);

    int x5 = center_x - radius * sin(0.4 * M_PI);
    int y5 = center_y - radius * cos(0.4 * M_PI);

    drawThickLine(image,x1,y1,x2+1,y2+1,thickness,colors,NULL,NULL,0,0);
    drawThickLine(image,x5,y5,x2+1,y2+1,thickness,colors,NULL,NULL,0,0);
    drawThickLine(image,x5,y5,x4+1,y4,thickness,colors,NULL,NULL,0,0);
    drawThickLine(image,x4,y4,x3-1,y3+1,thickness,colors,NULL,NULL,0,0);
    drawThickLine(image,x1,y1,x3-1,y3+1,thickness,colors,NULL,NULL,0,0);


}


char *find_value(Element *dict, int len, char *key) {
    for (int i = 0; i < len; i++) {
        if (strcmp(key, dict[i].key) == 0) {
            return dict[i].value;
        }
    }
}


int main(int argc, char *argv[]) {
    int last_index = 0;
    const char *short_options = "i:o:h";
    Element *dict = malloc(sizeof(Element) * 100);
    int len_dict = 0;


    const struct option long_options[] = {
            {"input",      required_argument, NULL, 'i'},
            {"output",     required_argument, NULL, 'o'},
            {"info",       no_argument,       NULL, 1},
            {"help",       no_argument,       NULL, 'h'},
            {"mirror",     no_argument,       NULL, 0},
            {"axis",       required_argument, NULL, 0},
            {"left_up",    required_argument, NULL, 0},
            {"right_down", required_argument, NULL, 0},
            {"pentagram",  no_argument,       NULL, 0},
            {"center",     required_argument, NULL, 0},
            {"radius",     required_argument, NULL, 0},
            {"thickness",  required_argument, NULL, 0},
            {"color",      required_argument, NULL, 0},
            {"fill",       required_argument, NULL, 0},
            {"fill_color", required_argument, NULL, 0},
            {"hexagon",    no_argument,       NULL, 0},
            {NULL, 0,                         NULL, 0}
    };

    int option;
    int option_index = 0;
    char *option_name;

    while ((option = getopt_long(argc, argv, short_options,
                                 long_options, &option_index)) != -1) {

        if (option == '?') {
            printf("unknown option\n");
            continue;
        }
        if (option == '1') {
            printf("%s\n", optarg);
        }

        option_name = strdup(long_options[option_index].name);
        if (optarg != NULL) {
            dict[len_dict].key = option_name;
            dict[len_dict++].value = optarg;
        } else {
            dict[len_dict].key = option_name;
            dict[len_dict++].value = "";
        }
    };
    int input_flag = 0;
    for (int i = 0; i < len_dict; i++) {
        if (strcmp("input", dict[i].key) == 0) {
            input_flag = 1;
        }
    }
    if (input_flag == 0) {
        if (optind < argc) {
            dict[len_dict].key = "input";
            dict[len_dict++].value = argv[optind - 1];
        } else {
            printf("Не передано имя входного файла\n");
        }
    }
//    printf("%s\n",argv[last_index+1]);
    PNGImage image;
    read_png_file("/Users/danilapachev/Desktop/projects/course_work/img.png", &image);

//    drawCircle(&image,500,500,20);


    char *color = "0.255.0";
    char *fill_color = "0.0.128";
    int colors[] = {0, 255, 255};
    colors[1] = 0;
    colors[2] = 0;
    colors[0] = 255;
//    drawSquare(&image,500,500,5,colors);
//    drawThickLine(&image,100,100,200,200,2,colors,NULL,NULL,0,0);
//    fill_circle(&image,500,500,2,colors);
//    drawThickLine(image.row_pointers,100,99,250,400,colors,NULL,NULL,0,0);
//    draw_rectangle(&image, 10, 10, 100, 200, 5, color, 1, fill_color);
//    mirror_image(&image, 'x', 30, 30, 1920, 1226);
//    draw_hexagon(&image, 1910, 500, 20, 4, color, 1, fill_color,0);
    color = "128.0.0";
//    draw_hexagon(&image, 500, 500, 16, 2, color, 0, fill_color,0);
//    draw_hexagon(&image, 500, 500, 15, 2, color, 0, fill_color,0);
//    draw_hexagon(&image, 500, 500, 15, 1, color, 0, fill_color,0);
//    draw_hexagon(&image, 500, 500, 14, 3, color, 0, fill_color,0);
//    draw_hexagon(&image, 500, 500, 13, 1, color, 0, fill_color,0);
//    draw_hexagon(&image, 500, 500, 12, 1, color, 0, fill_color,0);
//    draw_hexagon(&image, 500, 500, 11, 1, color, 0, fill_color,0);
//    draw_hexagon(&image, 500, 500, 11, 1, color, 0, fill_color,0);
//    draw_circle(&image, 200, 500, 30, 9, colors);
    pentagram(&image,1000,500,300,15,colors);
//    draw_line_square(&image,100,100,200,200,2,colors);
//    drawThickLine(&image,0,0,1000,1000,3,colors,NULL,NULL,0,0);
//    fill_circle(&image,1000,1000,4,colors,NULL,NULL,0,0);
//    drawSquare(&image,500,500,3,colors);
    write_png_file("/Users/danilapachev/Desktop/projects/course_work/out.png", &image);

    return 0;
}