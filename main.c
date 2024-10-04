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
    gif_init_global_state_color_map(&gif_global_state, global_color_map_buffer, global_color_map_size);
    printf("Color map: %d\n", global_color_map_buffer[global_color_map_size - 1]);

    while (true)
    {
        uint16_t extension_block_label;
        uint8_t extension_block_sub_block_size;

        while ((extension_block_label = gif_is_extension_block(&gif_global_state)))
        {
            switch (extension_block_label)
            {
            case gif_application_extension_label:
            {
                struct gif_application_extension_block_t extension;
                gif_get_extension_block(&gif_global_state, &extension);
                printf("Application extension identifier: %.*s\n", 8, extension.application_identifier);
                printf("Application extension auth code: %.*s\n", 3, extension.application_authentication_code);

                while ((extension_block_sub_block_size = gif_get_extension_block_sub_blocks_size(&gif_global_state)))
                {
                    uint8_t sub_blocks[extension_block_sub_block_size];
                    gif_get_extension_block_sub_blocks(&gif_global_state, extension_block_sub_block_size, sub_blocks);
                    printf("Sub blocks' size: %d\n", extension_block_sub_block_size);
                }

                break;
            }
            case gif_comment_extension_label:
            {
                printf("--- Start comment extension data --- \n");
                while ((extension_block_sub_block_size = gif_get_extension_block_sub_blocks_size(&gif_global_state)))
                {
                    uint8_t sub_blocks[extension_block_sub_block_size];
                    gif_get_extension_block_sub_blocks(&gif_global_state, extension_block_sub_block_size, sub_blocks);
                    printf("%.*s", extension_block_sub_block_size, sub_blocks);
                }
                printf("\n--- Finis comment extension data ---\n");
                break;
            }
            case gif_graphic_control_extension_label:
            {
                struct gif_graphic_control_extension_block_t extension;
                gif_get_extension_block(&gif_global_state, &extension);
                printf("Graphic control extension size: %d\n", extension.delay_time);
                if (gif_get_extension_block_sub_blocks_size(&gif_global_state))
                {
                    fprintf(stderr, "Graphic control extension should not have sub data");
                    exit(EXIT_FAILURE);
                }
                break;
            }
            }
        }

        struct gif_image_descriptor_t image_descriptor;
        gif_get_image_descriptor(&gif_global_state, &image_descriptor);
        printf("Image descriptor width: %d\n", image_descriptor.width);
        printf("Image descriptor height: %d\n", image_descriptor.height);

        printf("Finished frame\n");
    }
exit:
    fclose(file);

    return 0;
}
