#include "mirror_function.h"

void mirror_image(PNGImage *image, char axis, int x1, int y1, int x2, int y2) {
    if (axis == 'x') {
        if (y2>image->height){
            y2 = image->height;
        }
        if (x2>image->width){
            x2 = image->width;
        }
        if (y1<0){
            y1 = 0;
        }
        if (x1<0){
            x1 = 0;
        }
        if (x1>=image->width){
            return;
        }
        if (y1>=image->height){
            return;
        }

        for (int y = y1; y < y2; y++) {
            png_byte *row = image->row_pointers[y];
            int i = 0;
            for (int x = x1; x < round((x1+x2)/2); x++) {
                png_byte *ptr1 = &(row[x * 3]);

                int color1 = ptr1[0];
                int color2 = ptr1[1];
                int color3 = ptr1[2];
                png_byte *ptr2 = &(row[(x2 - i++) * 3]);
                ptr1[0] = ptr2[0];
                ptr1[1] = ptr2[1];
                ptr1[2] = ptr2[2];
                ptr2[0] = color1;
                ptr2[1] = color2;
                ptr2[2] = color3;
            }
        }
    } else if (axis == 'y') {

        if (y2>image->height){
            y2 = image->height-1;
        }
        if (x2>image->width){
            x2 = image->width-1;
        }
        if (y1<0){
            y1 = 0;
        }
        if (x1<0){
            x1 = 0;
        }
        if (x1>=image->width){
            return;
        }
        if (y1>=image->height){
            return;
        }

        for (int y = y1; y < round((y2+y1)/2); y++) {
            png_byte *row = image->row_pointers[y];
            png_byte *row2 = image->row_pointers[y2 - y+y1];
            for (int x = x1; x < x2; x++) {
                png_byte *ptr1 = &(row[x * 3]);
                int color1 = ptr1[0];
                int color2 = ptr1[1];
                int color3 = ptr1[2];
                png_byte *ptr2 = &(row2[x * 3]);
                ptr1[0] = ptr2[0];
                ptr1[1] = ptr2[1];
                ptr1[2] = ptr2[2];
                ptr2[0] = color1;
                ptr2[1] = color2;
                ptr2[2] = color3;
            }
        }
    }
}
