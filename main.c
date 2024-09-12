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

    struct gif_global_state_t gif_global_state;
    gif_init_global_state(gif_data, &gif_global_state);

    struct gif_header_t header = gif_global_state.header;
    printf("Gif signature: %.*s\n", 3, header.signature);
    printf("Gif version: %.*s\n", 3, header.version);

    struct gif_logical_screen_descriptor_t logical_screen_descriptor = gif_global_state.logical_screen_descriptor;
    printf("Width: %d\n", logical_screen_descriptor.width);
    printf("Height: %d\n", logical_screen_descriptor.height);

    uint16_t global_color_map_size = gif_get_global_color_table_size(&gif_global_state);
    uint8_t global_color_map_buffer[global_color_map_size];
    gif_init_global_state_color_map(&gif_global_state, global_color_map_size, global_color_map_buffer);
    printf("Color map: %d\n", global_color_map_buffer[global_color_map_size - 1]);

    while (true)
    {
        while (gif_is_extension_block(&gif_global_state))
        {
            struct gif_extension_block_t extension_block;
            gif_get_extension_block(&gif_global_state, &extension_block);

            switch (extension_block.header)
            {
            case gif_application_extension_label:
            {
                printf("Application extension size: %d\n", extension_block.application.block_size);
                printf("Application extension identifier: %.*s\n", 8, extension_block.application.application_identifier);
                printf("Application extension auth code: %.*s\n", 3, extension_block.application.application_authentication_code);

                while (extension_block.application.sub_blocks_block_size)
                {
                    uint8_t sub_blocks[extension_block.application.sub_blocks_block_size];
                    printf("Application extension sub blocks: %d\n", extension_block.application.sub_blocks_block_size);
                    gif_get_extension_block_sub_blocks(&gif_global_state, &extension_block, sub_blocks);
                }

                break;
            }
            case gif_comment_extension_label:
            {
                fprintf(stderr, "gif comment extension not implemented yet");
                exit(EXIT_FAILURE);
                break;
            }
            case gif_graphic_control_extension_label:
            {
                printf("Graphic control extension size: %d\n", extension_block.graphic_control.block_size);
                break;
            }
            }
        }
    }
exit:
    fclose(file);

    return 0;
}
