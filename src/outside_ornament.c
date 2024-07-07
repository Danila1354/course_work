#include "outside_ornament.h"
void outside_ornament(PNGImage* image, int thickness, char* color){
    int* colors = parse_color(color);
    int new_width = image->width+2*thickness;
    int new_height = image->height+2*thickness;
    png_bytep *row_pointers_copy = (png_bytep *) malloc(sizeof(png_bytep) * new_height);
    int k = 0;
    for (int y = 0; y < new_height; y++) {
        row_pointers_copy[k++] = (png_byte *) malloc(new_width*3);
    }
    for (int y = 0;y<thickness;y++){
        for (int x = 0;x<new_width;x++){
            row_pointers_copy[y][x*3] = colors[0];
            row_pointers_copy[y][x*3+1] = colors[1];
            row_pointers_copy[y][x*3+2] = colors[2];
        }
    }
    for (int y = 0;y<new_height;y++){
        for (int x = 0;x<thickness;x++){
            row_pointers_copy[y][x*3] = colors[0];
            row_pointers_copy[y][x*3+1] = colors[1];
            row_pointers_copy[y][x*3+2] = colors[2];
        }
    }
    for (int y = new_height-thickness;y<new_height;y++){
        for (int x = 0;x<new_width;x++){
            row_pointers_copy[y][x*3] = colors[0];
            row_pointers_copy[y][x*3+1] = colors[1];
            row_pointers_copy[y][x*3+2] = colors[2];
        }
    }
    for (int y = 0;y<new_height;y++){
        for (int x = new_width-thickness;x<new_width;x++){
            row_pointers_copy[y][x*3] = colors[0];
            row_pointers_copy[y][x*3+1] = colors[1];
            row_pointers_copy[y][x*3+2] = colors[2];
        }
    }
    int y1 = 0;
    int x1 = 0;
    for (int y = thickness;y<new_height-thickness;y++){
        for (int x = thickness;x<new_width-thickness;x++){
            row_pointers_copy[y][x*3] = image->row_pointers[y1][x*3];
            row_pointers_copy[y][x*3+1] = image->row_pointers[y1][x*3+1];
            row_pointers_copy[y][x*3+2] = image->row_pointers[y1][x*3+2];
            x1++;
        }
        y1++;
    }
    image->height = new_height;
    image->width = new_width;
    image->row_pointers = row_pointers_copy;

}