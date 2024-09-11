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
    printf("Gif signature: %.*s\n", (int)sizeof(header.signature), header.signature);
    printf("Gif version: %.*s\n", (int)sizeof(header.version), header.version);

    struct gif_logical_screen_descriptor_t logical_screen_descriptor;
    gif_get_logical_screen_descriptor(gif_data, &logical_screen_descriptor);
    printf("Width: %d\n", logical_screen_descriptor.width);
    printf("Height: %d\n", logical_screen_descriptor.height);

    struct gif_global_state_t gif_global_state;

    uint16_t global_color_map_size = gif_get_global_color_table_size(&logical_screen_descriptor);
    uint8_t global_color_map_memory[global_color_map_size];

    gif_global_state.color_map_size = global_color_map_size;
    gif_global_state.color_map_memory = global_color_map_memory;

    gif_get_global_state(gif_data, &gif_global_state);
    printf("Global offset: %x\n", *gif_global_state.static_offset);

    if (gif_is_special_purpose_block(gif_data, &gif_global_state))
    {
        struct special_purpose_block_t special_purpose_block;
        gif_get_special_purpose_block(gif_data, &gif_global_state, &special_purpose_block);

        switch (special_purpose_block.header)
        {
        case gif_application_extension:
        {
            printf("Application extension identifier: %.*s\n", 8, special_purpose_block.application_extension.application_identifier);
            printf("Application extension auth code: %.*s\n", 3, special_purpose_block.application_extension.application_authentication_code);

            while (special_purpose_block.application_extension.sub_blocks_block_size)
            {
                uint8_t sub_blocks[special_purpose_block.application_extension.sub_blocks_block_size];
                gif_get_special_purpose_block_sub_blocks(gif_data, &gif_global_state, &special_purpose_block, sub_blocks);
                printf(
                    "Application extension sub blocks: %.*s\n",
                    special_purpose_block.application_extension.sub_blocks_block_size,
                    sub_blocks);
            }

            break;
        }
        case gif_comment_extension:
        {
            fprintf(stderr, "gif comment extension not implemented yet");
            exit(EXIT_FAILURE);
            break;
        }
        }
    }

    printf("Dynamic offset: %d\n", *gif_global_state.dynamic_offset);
exit:
    fclose(file);

    return 0;
}
