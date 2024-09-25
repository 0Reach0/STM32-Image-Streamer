#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_rgb565> <output_array>\n", argv[0]);
        return 1;
    }
    
    const char *input_filename = argv[1]; 
    const char *output_filename = argv[2];

    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        perror("Failed to open input file");
        return 1;
    }

    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    if (file_size % 2 != 0) {
        fprintf(stderr, "File size must be a multiple of 2 for RGB565 format\n");
        fclose(input_file);
        return 1;
    }

    size_t pixel_count = file_size / 2; 
    uint16_t *image_data = (uint16_t *)malloc(pixel_count * sizeof(uint16_t));
    if (!image_data) {
        perror("Failed to allocate memory");
        fclose(input_file);
        return 1;
    }

    fread(image_data, sizeof(uint16_t), pixel_count, input_file);
    fclose(input_file); 

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        perror("Failed to open output file");
        free(image_data);
        return 1;
    }


    free(image_data);
    fclose(output_file);

    printf("Data successfully written to file %s\n", output_filename);
    return 0;
}
