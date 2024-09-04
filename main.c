#include <stdio.h>
#include <stdlib.h>

#include <my_tiny_gif.h>

int main(int argc, char const *argv[])
{
    FILE *file = fopen("../sample/homer.gif", "rb");
    if (!file)
    {
        fprintf(stderr, "File can't be open");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    uint64_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    uint8_t gif_data[size];
    fread(gif_data, size, size, file);

    struct gif_signature_t signature;
    gif_get_signature(gif_data, &signature);

    printf("Gif signature: %s\n", signature.signature);

    uint16_t width = gif_screen_width(gif_data);
    uint16_t height = gif_screen_height(gif_data);

    printf("Width %d\n", width);
    printf("Height %d\n", height);

    if (gif_has_sort_flag(gif_data))
    {
        printf("Gif has sort flag");
    }

    uint8_t bits_resolution = gif_color_resolution_bits(gif_data);
    printf("Bits resolution %d\n", bits_resolution);

    uint8_t global_color_table_bits = gif_global_color_table_bits(gif_data);
    printf("Color table bits resolution %d\n", global_color_table_bits);

    if (gif_has_global_color_table_flag(gif_data))
    {
        printf("Gif has global color map");
        uint16_t global_color_table_size = gif_global_color_table_size(global_color_table_bits);
        printf("Global color table size will be %d\n", global_color_table_size);
        uint8_t global_color_table[global_color_table_size];
    }

exit:
    fclose(file);

    return 0;
}
