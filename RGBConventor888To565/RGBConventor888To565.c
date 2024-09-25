#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <png.h>
#include <jpeglib.h>
// uint16_t rgb888torgb565(uint8_t *rgb888Pixel) {
//     uint8_t red = rgb888Pixel[0];   
//     uint8_t green = rgb888Pixel[1]; 
//     uint8_t blue = rgb888Pixel[2];  

//     uint16_t r5 = (red >> 3) & 0x1F;    
//     uint16_t g6 = (green >> 2) & 0x3F;  
//     uint16_t b5 = (blue >> 3) & 0x1F;   

//     return (r5 << 11) | (g6 << 5) | b5; 
// }

// void rgb565torgb888(uint16_t rgb565, uint8_t *rgb888Pixel) {
//     uint8_t r5 = (rgb565 >> 11) & 0x1F; 
//     uint8_t g6 = (rgb565 >> 5) & 0x3F;
//     uint8_t b5 = rgb565 & 0x1F;         

//     rgb888Pixel[0] = (r5 << 3) | (r5 >> 2); 
//     rgb888Pixel[1] = (g6 << 2) | (g6 >> 4); 
//     rgb888Pixel[2] = (b5 << 3) | (b5 >> 2); 
// }

    

uint16_t rgb888torgb565(uint8_t *rgb888Pixel) {
    uint8_t red = rgb888Pixel[0];  
    uint8_t green = rgb888Pixel[1];
    uint8_t blue = rgb888Pixel[2];  

    uint16_t r5 = (red >> 3) & 0x1F;    
    uint16_t g6 = (green >> 2) & 0x3F;  
    uint16_t b5 = (blue >> 3) & 0x1F;   

    return (r5 << 11) | (g6 << 5) | b5; 
}

void read_jpeg_file(const char* filename, unsigned char ***row_pointers, int *width, int *height) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Failed to open JPEG file");
        exit(EXIT_FAILURE);
    }

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, fp);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    *width = cinfo.output_width;
    *height = cinfo.output_height;
    int channels = cinfo.output_components;  // 3 for RGB, 1 for grayscale

    *row_pointers = (unsigned char **)malloc(*height * sizeof(unsigned char *));
    for (int y = 0; y < *height; y++) {
        (*row_pointers)[y] = (unsigned char *)malloc(*width * channels);
    }

    while (cinfo.output_scanline < cinfo.output_height) {
        unsigned char *buffer_array[1];
        buffer_array[0] = (*row_pointers)[cinfo.output_scanline];
        jpeg_read_scanlines(&cinfo, buffer_array, 1);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(fp);
}

void read_png_file(const char* filename, png_bytep **row_pointers, int *width, int *height) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Failed to open PNG file");
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort();

    png_infop info = png_create_info_struct(png);
    if (!info) abort();

    if (setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    png_read_info(png, info);

    *width = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16)
        png_set_strip_16(png);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        png_set_filler(png, 0, PNG_FILLER_BEFORE);

    png_read_update_info(png, info);

    *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * (*height));
    for (int y = 0; y < *height; y++) {
        (*row_pointers)[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, *row_pointers);

    fclose(fp);
    png_destroy_read_struct(&png, &info, NULL);
}

int main(int argc, char *argv[]) {
        if (argc < 3) {
        fprintf(stderr, "At least 2 arguments: %s <input_png> <output_rgb565>\n", argv[0]);
        return 1;
    }
    char  file_type = 'p';
    if(argc > 4)
    {
        file_type = *argv[3];
        if(file_type != 'p' && file_type != 'j')
        {
            fprintf(stderr, "3 argument is file type: %s <p - png>  or <j - jpeg>\n", argv[0]);
        }
    }

    const char *file = argv[1];
    const char *output_file = argv[2];

    png_bytep *row_pointers;
    int width, height;
    if(file_type == 'p') read_png_file(file, &row_pointers, &width, &height);
    else read_jpeg_file(file, &row_pointers, &width, &height);
    FILE *tmp = fopen(output_file, "wb");
    if (!tmp) {
        perror("Failed to open output file");
        return 1;
    }

    for (int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 3]); 
            uint16_t rgb565 = rgb888torgb565(px);
            fwrite(&rgb565, sizeof(uint16_t), 1, tmp);
        }
        free(row_pointers[y]);
    }
    free(row_pointers);

    fclose(tmp);
    return 0;
}
