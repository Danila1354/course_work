#include "png_functions.h"

void read_png_file(char *file_name, PNGImage *image) {
    int x, y;
    char header[8]; // 8 is the maximum size that can be checked

    FILE *fp = fopen(file_name, "rb");
    if (!fp) {
        fprintf(stderr,"Error: file can not be open\n");
        exit(45);
    }

    fread(header, 1, 8, fp);
    if (png_sig_cmp((png_const_bytep) (header), 0, 8)) {
        fprintf(stderr, "Error: File is not recognized as a PNG.\n");
        exit(45);
    }

    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!image->png_ptr) {
        fprintf(stderr, "Error: png_create_read_struct failed\n");
        exit(45);
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr) {
        png_destroy_read_struct(&image->png_ptr, NULL, NULL);
        exit(0);
    }

    if (setjmp(png_jmpbuf(image->png_ptr))) {
        fprintf(stderr, "Error: error during init_io\n");
        exit(45);
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
    if (setjmp(png_jmpbuf(image->png_ptr))) {
        fprintf(stderr,"Похоже в картинке битые пиксели\n");
        exit(45);
    }
    image->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * image->height);
    for (y = 0; y < image->height; y++)
        image->row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));
    png_read_image(image->png_ptr, image->row_pointers);
}
void write_png_file(char *file_name, PNGImage *image) {
    FILE *fp = fopen(file_name, "wb");
    if (!fp) {
        fprintf(stderr,"Can not open file for write\n");
        exit(45);
    }
    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!image->png_ptr) {
        fprintf(stderr,"Error: png_create_write_struct failed\n");
        exit(45);
    }
    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr) {
        fprintf(stderr,"Error: png_create_info_struct failed\n");
        exit(45);
    }
    png_init_io(image->png_ptr, fp);
    if (setjmp(png_jmpbuf(image->png_ptr))) {
        fprintf(stderr,"Error: error during writing header\n");
        exit(45);
    }
    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height, image->bit_depth, image->color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
    png_write_info(image->png_ptr, image->info_ptr);
    if (setjmp(png_jmpbuf(image->png_ptr))) {
        fprintf(stderr,"Error: error during writing bytes\n");
        exit(45);
    }
    png_write_image(image->png_ptr, image->row_pointers);
    if (setjmp(png_jmpbuf(image->png_ptr))) {
        fprintf(stderr,"Error: error during end of write\n");
        exit(45);
    }
    png_write_end(image->png_ptr, NULL);
    for (int y = 0; y < image->height; y++) {
        free(image->row_pointers[y]);
    }
    free(image->row_pointers);
    fclose(fp);
}