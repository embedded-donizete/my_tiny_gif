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

    while (gif_is_special_purpose_block(&gif_global_state))
    {
        struct gif_special_purpose_block_t special_purpose_block;
        gif_get_special_purpose_block(&gif_global_state, &special_purpose_block);

        switch (special_purpose_block.header)
        {
        case gif_special_purpose_block_application_label:
        {
            printf("Application extension identifier: %.*s\n", 8, special_purpose_block.application_extension.application_identifier);
            printf("Application extension auth code: %.*s\n", 3, special_purpose_block.application_extension.application_authentication_code);

            while (special_purpose_block.application_extension.sub_blocks_block_size)
            {
                uint8_t sub_blocks[special_purpose_block.application_extension.sub_blocks_block_size];
                gif_get_special_purpose_block_sub_blocks(&gif_global_state, &special_purpose_block, sub_blocks);
                printf(
                    "Application extension sub blocks: %.*s\n",
                    special_purpose_block.application_extension.sub_blocks_block_size,
                    sub_blocks);
            }

            break;
        }
        case gif_special_purpose_block_comment_label:
        {
            fprintf(stderr, "gif comment extension not implemented yet");
            exit(EXIT_FAILURE);
            break;
        }
        }
    }

    while (gif_is_graphic_control_extension(&gif_global_state))
    {
        printf("AAAA");
    }
exit:
    fclose(file);

    return 0;
}
