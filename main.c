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

    struct gif_signature_t signature;
    gif_get_signature(gif_data, &signature);
    printf("Gif signature: %s\n", signature.buffer);

    struct gif_screen_descriptor_t screen_descriptor;
    gif_get_screen_descriptor(gif_data, &screen_descriptor);
    printf("Width: %d\n", screen_descriptor.width);
    printf("Height: %d\n", screen_descriptor.height);

    struct gif_state_t gif_state;

    uint16_t global_color_map_size = gif_get_global_color_table_size(&screen_descriptor);
    uint8_t global_color_map_memory[global_color_map_size];

    gif_state.global_color_map_size = global_color_map_size;
    gif_state.global_color_map_memory = global_color_map_memory;

    gif_start_state(gif_data, &gif_state);
    printf("Global color map size: %d\n", gif_state.global_color_map_size);
    printf("%d\n", global_color_map_memory[global_color_map_size - 1]);

exit:
    fclose(file);

    return 0;
}
