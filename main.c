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

    if (!gif_has_a_valid_header(gif_data))
    {
        fprintf(stderr, "Is it a GIF?");
        goto exit;
    }

    uint8_t version[4] = {0};
    gif_signature_version(gif_data, version);
    printf("Version %s\n", version);

    uint16_t width = gif_screen_width(gif_data);
    uint16_t height = gif_screen_height(gif_data);

    printf("Width %d\n", width);
    printf("Height %d\n", height);

    if (gif_has_global_color_table_flag(gif_data))
    {
        printf("Gif has global color map");
    }

    if (gif_has_sort_flag(gif_data))
    {
        printf("Gif has sort flag");
    }

    uint8_t bits_resolution = gif_color_resolution_bits(gif_data);
    printf("Bits resolution %d\n", bits_resolution);

    uint8_t color_table_bits = gif_global_color_table_bits(gif_data);
    printf("Color table bits resolution %d\n", color_table_bits);

exit:
    fclose(file);

    return 0;
}
