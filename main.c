#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    struct gif_header_t header;
    gif_get_header(gif_data, &header);
    printf("Gif signature: %.*s\n", (int) sizeof(header.signature), header.signature);
    printf("Gif version: %.*s\n", (int) sizeof(header.version), header.version);

    struct gif_logical_screen_descriptor_t logical_screen_descriptor;
    gif_get_logical_screen_descriptor(gif_data, &logical_screen_descriptor);
    printf("Width: %d\n", logical_screen_descriptor.width);
    printf("Height: %d\n", logical_screen_descriptor.height);

    struct gif_global_state_t gif_global_state;

    uint16_t global_color_map_size = gif_get_global_color_table_size(&logical_screen_descriptor);
    uint8_t global_color_map_memory[global_color_map_size];

    gif_global_state.global_color_map_size = global_color_map_size;
    gif_global_state.global_color_map_memory = global_color_map_memory;

exit:
    fclose(file);

    return 0;
}
