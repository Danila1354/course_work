#include <stdio.h>
#include </opt/homebrew/opt/libpng/include/png.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
typedef struct Element{
    char* key;
    char* value;
} Element;
typedef struct {
    int x, y;
} Point;

//int isInsidePolygon(Point polygon[], int n, Point p) {
//    int i, j, c = 0;
//    for (i = 0, j = n-1; i < n; j = i++) {
//        if (((polygon[i].y > p.y) != (polygon[j].y > p.y)) &&
//            (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x))
//            c = !c;
//    }
//    return c;
//}
//
//int check_coords(int x1, int y1, int x2, int y2,int width,int height) {
//    if (x1<0||x2<0||y1<0 || y2<0 || x1 > x2 || y1 > y2 || (x1>width) || (y1>height)){
//        return 0;
//    }
//    return 1;
//}
//int check_el_in_array(int *array, int el, int size){
//    for (int i = 0; i<size;i++){
//        if (array[i]==el){
//            return 1;
//        }
//    }
//    return 0;
//}
//int find_y_coord(int center_y,int center_x,int x,int radius,int x_for_b,int y_for_b,int option){
//    int y;
//    if (option == 3){
//        int b = y_for_b+(sqrt(3)/3)*x_for_b;
//        y = round((-sqrt(3)/3)*x + (b));
//    }
//    else if (option == 2){
//        int b = y_for_b-(sqrt(3)/3)*x_for_b;
//        y = round((sqrt(3)/3)*x+b);
//    }
//    else if (option == 5){
//        int b = (sqrt(3)/3)*x_for_b - y_for_b;
//        y = round((sqrt(3)/3)*x-b);
//    }
//    else if(option == 6){
//        int b =(-sqrt(3)/3)*x_for_b - y_for_b;
//        y = round((-sqrt(3)/3)*x - (b));
//    }
//    return y;
//}
//int find_x_coord(int center_y,int center_x,int y,int radius,int x_for_b,int y_for_b,int option) {
//    int x;
//    if (option == 3) {
//        int b = y_for_b + (sqrt(3) / 3) * x_for_b;
//        x = round((y-b)/(-sqrt(3) / 3));
//    } else if (option == 2) {
//        int b = y_for_b - (sqrt(3) / 3) * x_for_b;
//        x = round((y-b)/(sqrt(3) / 3));
//    } else if (option == 5) {
//        int b = (sqrt(3) / 3) * x_for_b - y_for_b;
//        x = round((y+b)/(sqrt(3) / 3));
//    } else if (option == 6) {
//        int b = (-sqrt(3) / 3) * x_for_b - y_for_b;
//        x = round((y+b)/(-sqrt(3) / 3));
//    }
//    return x;
//}
typedef struct PNGImage {
    int width, height; // width and height of the image
    png_byte color_type; // color type of the image
    png_byte bit_depth; // bit depth of the image
    png_structp png_ptr; // pointer to the png struct
    png_infop info_ptr; // pointer to the png info struct
    int number_of_passes;
    png_bytep *row_pointers;


} PNGImage;
void dilation(png_bytep *row_pointers, int x, int y,int thickness,int* colors){
    for (int i = 1;i<thickness;i++){
        png_byte *ptr1 = &(row_pointers[y][(x-i) * 3]);
        png_byte *ptr2 = &(row_pointers[y-i][x * 3]);
        png_byte *ptr3 = &(row_pointers[y][(x+i) * 3]);
        png_byte *ptr4 = &(row_pointers[y+i][x * 3]);
        ptr1[0] = colors[0];
        ptr1[1] = colors[1];
        ptr1[2] = colors[2];
        ptr2[0] = colors[0];
        ptr2[1] = colors[1];
        ptr2[2] = colors[2];
        ptr3[0] = colors[0];
        ptr3[1] = colors[1];
        ptr3[2] = colors[2];
        ptr4[0] = colors[0];
        ptr4[1] = colors[1];
        ptr4[2] = colors[2];
    }
}
void drawThickLine(png_bytep *row_pointers, int x1, int y1, int x2, int y2,int thickness, int *colors,int* fill_x_array,int* fill_y_array,int* len_x,int* len_y) {
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
        png_byte *ptr = &(row_pointers[y][x * 3]);
        ptr[0] = colors[0];
        ptr[1] = colors[1];
        ptr[2] = colors[2];
        dilation(row_pointers,x,y,thickness,colors);
        if (fill_x_array!=NULL && fill_y_array!=NULL){
            fill_x_array[(*len_x)] = x;
            fill_y_array[(*len_y)] = y;
            (*len_x)++;
            (*len_y)++;
        }
        e2 = 190 * err;
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



//// убрать отсюда функцию
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
//void draw_rectangle(PNGImage *image, int x1, int y1, int x2, int y2, int thickness, char *color, int fill,
//                    char *fill_color) {
//    if (check_coords(x1, y1, x2, y2,image->width,image->height) == 0) {
//        return;
//    }
//    char* copy_color = strdup(color);
//
//    char *token = strtok(copy_color, ".");
//    int colors[3];
//    for (int i = 0; i < 3; i++) {
//        colors[i] = atoi(token);
//        token = strtok(NULL, ".");
//    }
//    int fill_colors[3];
//    char* copy_fill_color = strdup(fill_color);
//    token = strtok(copy_fill_color, ".");
//    for (int i = 0; i < 3; i++) {
//        fill_colors[i] = atoi(token);
//        token = strtok(NULL, ".");
//    }
//    if (thickness>0){
//        int x1_coord;
//        int y1_coord;
//        int x2_coord;
//        int y2_coord;
//        //draw top line
//        for (int i = 0; i < thickness; i++) {
//            if (x1<0){
//                x1 = 0;
//            }
//            if (y1<0){
//                y1 = 0;
//            }
//            y1_coord = y1 + i;
//            if (y1_coord >= image->height){
//                continue;
//            }
//            x2_coord = x2;
//            if (x2_coord > image->width){
//                x2_coord = image->width;
//            }
//            y2_coord = y1 + i;
//            drawThickLine(image->row_pointers, x1, y1_coord, x2_coord, y2_coord, colors, NULL, NULL, 0, 0);
//        }
//        //draw bottom line
//        for (int i = 0; i < thickness; i++) {
//            y1_coord = y2 - i;
//            if (y1_coord >= image->height || y1_coord<0){
//                continue;
//            }
//            x2_coord = x2;
//            if (x2_coord > image->width){
//                x2_coord = image->width;
//            }
//            y2_coord = y2 - i;
//            drawThickLine(image->row_pointers, x1, y1_coord, x2_coord, y2_coord, colors, NULL, NULL, 0, 0);
//        }
//        // draw left line
//        for (int i = 0; i < thickness; i++) {
//            x1_coord = x1 + i;
//            if (x1_coord >= image->width){
//                continue;
//            }
//            x2_coord = x1 + i;
//            y2_coord = y2+1;
//            if (y2_coord> image->height){
//                y2_coord = image->height;
//            }
//
//            drawThickLine(image->row_pointers, x1_coord, y1, x2_coord, y2_coord, colors, NULL, NULL, 0, 0);
//
//        }
//        // draw right line
//        for (int i = 0; i < thickness; i++) {
//            x1_coord = x2 - i;
//            if (x1_coord >= image->width || x1_coord<0){
//                continue;
//            }
//
//            x2_coord = x2 - i;
//            y2_coord = y2+1;
//            if (y2_coord> image->height){
//                y2_coord = image->height;
//            }
//            drawThickLine(image->row_pointers, x1_coord, y1, x2_coord, y2_coord, colors, NULL, NULL, 0, 0);
//
//        }
//    }
//    if (fill) {
//        for (int y = y1+thickness; y < y2-thickness+1; y++) {
//            png_byte *row;
//            if (y>=image->height){
//                continue;
//            }
//            row = image->row_pointers[y];
//            for (int x = x1+thickness; x < x2-thickness+1; x++) {
//                if (x>=image->width){
//                    continue;
//                }
//                png_byte *ptr = &(row[x * 3]);
//                ptr[0] = fill_colors[0];
//                ptr[1] = fill_colors[1];
//                ptr[2] = fill_colors[2];
//            }
//        }
//    }
//    free(copy_color);
//    free(copy_fill_color);
//
//}
//
//
//
//
//void draw_hexagon(PNGImage *image,int center_x, int center_y, int radius, int thickness, char *color, int fill, char *fill_color) {
//    // check radius and thickness
//    char* copy_color = strdup(color);
//    char *token = strtok(copy_color, ".");
//    int colors[3];
//    for (int i = 0; i < 3; i++) {
//        colors[i] = atoi(token);
//        token = strtok(NULL, ".");
//    }
//    int fill_colors[3];
//    char* copy_fill_color = strdup(fill_color);
//    token = strtok(copy_fill_color, ".");
//    for (int i = 0; i < 3; i++) {
//        fill_colors[i] = atoi(token);
//        token = strtok(NULL, ".");
//    }
//    if (radius%2==1){
//        radius++;
//    }
//
//
//    int x_coords_array[10024];
//    int y_coords_array[10024];
//    int x_array_len = 0;
//    int y_array_len = 0;
//    int x1 = round(center_x - (sqrt(radius*radius - (radius/2)*(radius/2))));
//    int y1 = round(center_y - (radius/2));
//    int x2 = x1;
//    int y2 = round(center_y + (radius/2));
//    int x3 = center_x;
//    int y3 = center_y+radius;
//    int x4 = round(center_x + (sqrt(radius*radius - (radius/2)*(radius/2))));
//    int y4 = round(center_y +(radius/2));
//    int x5 = x4;
//    int y5 = round(center_y - (radius/2));
//    int x6 = center_x;
//    int y6 = center_y - radius;
//    Point hexagon[] = {{x1, y1}, {x2, y2},{x3,y3},{x4,y4},{x5,y5},{x6,y6}};
//    int x1_coord;
//    int y1_coord;
//    int x2_coord;
//    int y2_coord;
//    if (!(y3<0 || x4<0 || y6>=image->height || x1>=image->width || radius<=0)){
//        for (int i = 0;i<thickness;i++){
//            x1_coord = x1+i;
//            y1_coord = y1;
//            hexagon[0].x = x1_coord;
//            hexagon[0].y = y1;
//            if (x1_coord>=image->width || x1_coord<=0){
//                continue;
//            }
//
//            if (y1_coord>=image->height){
//                continue;
//            }
//
//            if (y1_coord<0){
//                y1_coord = 0;
//            }
//            x2_coord = x2+i;
//            if (x2_coord>image->width){
//                x2_coord = image->width;
//            }
//            y2_coord = y2;
//            if (y2_coord>image->height){
//                y2_coord = image->height;
//            }
//            if (y2_coord<0){
//                continue;
//            }
//
//            drawThickLine(image->row_pointers,x1_coord, y1_coord, x2_coord, y2_coord, colors,
//                          x_coords_array, y_coords_array, &x_array_len, &y_array_len);
//
//        }
//        for (int i = 0;i<thickness;i++){
//            x1_coord = x2;
//            y1_coord = y2-i-1;
//            hexagon[1].x = x2+i;
//            hexagon[1].y = y2-1;
//            if (x1_coord>=image->width){
//                continue;
//            }
//            if (y1_coord>=image->height){
//                continue;
//            }
//            if (x1_coord<0){
//                x1_coord = 0;
//                y1_coord = find_y_coord(center_y,center_x,x1_coord,radius,x2,y2-i,2);
//            }
//            if (y1_coord<0){
//                y1_coord = 0;
//                x1_coord = find_x_coord(center_y,center_x,y1_coord,radius,x2,y2-i,2);
//            }
//            x2_coord = x3+1;
//            y2_coord = y3-i+1;
//            if (x2_coord>image->width){
//                x2_coord = image->width;
//                y2_coord = find_y_coord(center_y,center_x,x2_coord,radius,x2,y2-i,2);
//            }
//            if (y2_coord>image->height){
//                y2_coord = image->height;
//                x2_coord = find_x_coord(center_y,center_x,y2_coord,radius,x2,y2-i,2);
//            }
//            if (x2_coord<0){
//                continue;
//            }
//            if (y2_coord<0){
//                continue;
//            }
//
//            drawThickLine(image->row_pointers,x1_coord, y1_coord, x2_coord, y2_coord, colors,
//                          x_coords_array, y_coords_array, &x_array_len, &y_array_len);
//        } // Istoped here --------------------------------------------------------------------------------------------------
//        for (int i = 0;i<thickness;i++){
//            x1_coord = x3;
//            y1_coord = y3-i;
//            hexagon[2].x = x1_coord;
//            hexagon[2].y = y1_coord;
//            if (x1_coord>=image->width){
//                continue;
//            }
//            if (y1_coord>=image->height){
//                y1_coord = image->height-1;
//                x1_coord = find_x_coord(center_y,center_x,y1_coord+1,radius,x3,y3-i,3);
//            }
//            if (x1_coord<0){
//                x1_coord = 0;
//                y1_coord = find_y_coord(center_y,center_x,x1_coord,radius,x3,y3-i,3);
//            }
//            if (y1_coord<0){
//                continue;
//            }
//            x2_coord = x4+1;
//            y2_coord = y4-i-2;
//            if (x2_coord>image->width){
//                x2_coord = image->width;
//                y2_coord = find_y_coord(center_y,center_x,x2_coord,radius,x3,y3-i,3);
//            }
//            if (x2_coord<0){
//                continue;
//            }
//            if (y2_coord<0){
//                y2_coord = -1;
//                x2_coord = find_x_coord(center_y,center_x,y2_coord,radius,x4,y4-i,3);
//            }
//            if (y2_coord>image->height){
//                continue;
//            }
//
//
//            drawThickLine(image->row_pointers,x1_coord, y1_coord, x2_coord, y2_coord, colors,
//                          x_coords_array, y_coords_array, &x_array_len, &y_array_len);
//        }
//        for (int i = 0;i<thickness;i++){
//            x1_coord = x5-i;
//            y1_coord = y5;
//            hexagon[3].x = x4-i;
//            hexagon[3].y = y4-1;
//            if (x1_coord>=image->width || x1_coord<0){
//                continue;
//            }
//            if (y1_coord>image->height){
//                continue;
//            }
//            if (y1_coord<0){
//                y1_coord = 0;
//            }
//            x2_coord = x4-i;
//            y2_coord = y4;
//            if (y2_coord<0){
//                y2_coord = -1;
//            }
//            if (y2_coord>image->height){
//                y2_coord = image->height;
//            }
//            drawThickLine(image->row_pointers,x1_coord, y1_coord, x2_coord, y2_coord, colors,
//                          x_coords_array, y_coords_array, &x_array_len, &y_array_len);
//        }
//        for (int i = 0;i<thickness;i++){
//            x1_coord = x6;
//            y1_coord = y6+i;
//            x2_coord = x5+1;
//            y2_coord = y5+i+1;
//            hexagon[4].x = x5-i;
//            hexagon[4].y = y5;
//            if (x1_coord>=image->width){
//                continue;
//            }
//            if (y1_coord>=image->height){
//                continue;
//            }
//            if (x1_coord<0){
//                x1_coord = 0;
//                y1_coord  = find_y_coord(center_y,center_x,x1_coord,radius,x6,y6+i,5);
//            }
//            if (y1_coord<0){
//                y1_coord = 0;
//                x1_coord = find_x_coord(center_y,center_x,y1_coord,radius,x6,y6+i,5);
//            }
//            if (x2_coord>image->width){
//                x2_coord = image->width;
//                y2_coord = find_y_coord(center_y,center_x,x2_coord,radius,x6,y6+i,5);
//            }
//            if (y2_coord>image->height){
//                y2_coord = image->height;
//                x2_coord = find_x_coord(center_y,center_x,y2_coord,radius,x6,y6+i,5);
//            }
//            if (y2_coord<0){
//                continue;
//            }
//            if (x2_coord<0){
//                x2_coord = -1;
//                y2_coord = find_y_coord(center_y,center_x,x2_coord+1,radius,x5,y5+i,5);
//            }
//
//            drawThickLine(image->row_pointers,x1_coord, y1_coord, x2_coord, y2_coord, colors,
//                          x_coords_array, y_coords_array, &x_array_len, &y_array_len);
//        }
//        for (int i = 0;i<thickness;i++){
//            x1_coord = x6;
//            y1_coord = y6+i;
//            x2_coord = x1-1;
//            y2_coord = y1+i+1;
//            int new_x1_coord;
//            int new_y1_coord;
//            int new_x2_coord;
//            int new_y2_coord;
//            hexagon[5].x = x6;
//            hexagon[5].y = y6+i;
//            if (x1_coord>=image->width){
//                x1_coord = image->width-1;
//                y1_coord = find_y_coord(center_y,center_x,x1_coord+1,radius,x6,y6+i,6);
//            }
//            if (y1_coord>=image->height){
//                continue;
//            }
//            if (x1_coord<0){
//                continue;
//            }
//            if (y1_coord<0){
//                y1_coord = 0;
//                x1_coord = find_x_coord(center_y,center_x,y1_coord,radius,x6,y6+i,6);
//            }
//            if (x2_coord<0){
//                x2_coord = -1;
//                y2_coord = find_y_coord(center_y,center_x,x2_coord+1,radius,x1,y1+i,6);
//            }
//            if (y2_coord<0){
//                continue;
//            }
//            if (x2_coord>image->width){
//                continue;
//            }
//            if (y2_coord>image->height){
//                y2_coord = image->height;
//                x2_coord = find_x_coord(center_y,center_x,y2_coord,radius,x1+i,y1,6);
//            }
//
//            drawThickLine(image->row_pointers,x1_coord, y1_coord, x2_coord, y2_coord, colors,
//                          x_coords_array, y_coords_array, &x_array_len, &y_array_len);
//        }
//
//        if (fill){
//
//            for (int y = y6-1;y<y3+1;y++){
//                for (int x = x1;x<x4+1;x++){
//                    int flag = 0;
//                    Point point = {x, y};
//                    for (int i = 0;i<x_array_len;i++){
//                        if (x==x_coords_array[i] && y==y_coords_array[i]){
//                            flag = 1;
//                            break;
//                        }
//                    }
//                    if (y>=image->height || x>=image->width || y<0 || x<0){
//                        continue;
//                    }
//                    if (flag==0 && isInsidePolygon(hexagon, 6, point)){
//                        png_byte *ptr = &(image->row_pointers[y][x * 3]);
//                        ptr[0] = fill_colors[0];
//                        ptr[1] = fill_colors[1];
//                        ptr[2] = fill_colors[2];
//                    }
//
//                }
//            }
//        }
//    }
//
//
//
//}
void setPixel(png_bytep *row_pointers, int x, int y) {
    row_pointers[y][x * 3] = 255;
    row_pointers[y][x * 3 + 1] = 0;
    row_pointers[y][x * 3 + 2] = 0;
}
void drawCircle(PNGImage *image, int x0, int y0, int radius) {

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
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
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
char* find_value(Element* dict,int len, char* key){
    for (int i = 0;i<len;i++){
        if (strcmp(key,dict[i].key)==0){
            return dict[i].value;
        }
    }
}


int main(int argc, char* argv[]) {
    int last_index = 0;
    const char* short_options = "i:o:h";
    Element* dict = malloc(sizeof(Element)*100);
    int len_dict = 0;


    const struct option long_options[] = {
            { "input", required_argument, NULL, 'i' },
            { "output", required_argument, NULL, 'o' },
            { "info", no_argument, NULL, 1 },
            { "help", no_argument, NULL, 'h' },
            { "mirror", no_argument, NULL, 0 },
            { "axis", required_argument, NULL, 0 },
            { "left_up", required_argument, NULL, 0 },
            { "right_down", required_argument, NULL, 0 },
            { "pentagram", no_argument, NULL, 0 },
            { "center", required_argument, NULL, 0 },
            { "radius", required_argument, NULL, 0 },
            { "thickness", required_argument, NULL, 0 },
            { "color", required_argument, NULL, 0 },
            { "fill", required_argument, NULL, 0 },
            { "fill_color", required_argument, NULL, 0 },
            { "hexagon", no_argument, NULL, 0 },
            { NULL, 0, NULL, 0 }
    };

    int option;
    int option_index = 0;
    char* option_name;

    while ((option=getopt_long(argc,argv,short_options,
                            long_options,&option_index))!=-1){

        if (option == '?') {
            printf("unknown option\n");
            continue;
        }
        if (option == '1'){
            printf("%s\n",optarg);
        }

        option_name = strdup(long_options[option_index].name);
        if (optarg!=NULL){
            dict[len_dict].key = option_name;
            dict[len_dict++].value = optarg;
        }
        else{
            dict[len_dict].key = option_name;
            dict[len_dict++].value = "";
        }
    };
    int input_flag = 0;
    for (int i = 0;i<len_dict;i++){
        if (strcmp("input",dict[i].key)==0){
            input_flag = 1;
        }
    }
    if (input_flag==0){
        if (optind<argc){
            dict[len_dict].key = "input";
            dict[len_dict++].value = argv[optind-1];
        }
        else{
            printf("Не передано имя входного файла\n");
        }
    }
//    printf("%s\n",argv[last_index+1]);
    PNGImage image;
    read_png_file("/Users/danilapachev/Desktop/projects/course_work/img.png", &image);

//    drawCircle(&image,500,500,20);


//    char *color = "0.255.0";
//    char *fill_color = "0.0.128";
    int colors[] = {0,255,255};
    drawThickLine(image.row_pointers,100,100,200,200,3,colors,NULL,NULL,0,0);
//    drawThickLine(image.row_pointers,99,100,250,400,colors,NULL,NULL,0,0);
//    drawThickLine(image.row_pointers,100,99,250,400,colors,NULL,NULL,0,0);

//    draw_rectangle(&image, 10, 10, 100, 100, 4, color, 1, fill_color);
//    drawThickLine(image.row_pointers,0,0,1912,1226,colors);
//    mirror_image(&image, 'x', 30, 30, 1920, 1226);
//    draw_hexagon(&image, 500, 20, 26, 20, color, 1, fill_color);
    write_png_file("/Users/danilapachev/Desktop/projects/course_work/out.png", &image);

    return 0;
}