#include <stdint.h>
#include <stdbool.h>

struct gif_header_t
{
    uint8_t signature[3];
    uint8_t version[3];
} __attribute__((packed));

enum gif_logical_screen_descriptor_packed_fields_bitwise_enum
{
    global_map_mask = 0b10000000,
    color_resolution_mask = 0b01110000,
    sort_mask_mask = 0b00001000,
    color_table_mask = 0b00000111,
};

struct gif_logical_screen_descriptor_t
{
    uint16_t width;
    uint16_t height;
    uint8_t packed_fields;
    uint8_t background_color_index;
    uint8_t pixel_aspect_ratio;
} __attribute__((packed));

struct gif_global_state_t
{
    const uint8_t *gif_pointer;

    struct gif_header_t header;
    struct gif_logical_screen_descriptor_t logical_screen_descriptor;

    uint8_t *color_map_buffer;
    uint16_t color_map_size;
};

void gif_init_global_state(const uint8_t *const _, struct gif_global_state_t *);
uint16_t gif_get_global_color_table_size(const struct gif_global_state_t *);
void gif_init_global_state_color_map(struct gif_global_state_t *, uint16_t, uint8_t *);

enum gif_extension_block_enum
{
    gif_application_extension_label = 0xFF21,
    gif_comment_extension_label = 0xFE21,
    gif_graphic_control_extension_label = 0xF921
};

struct gif_application_extension_block_t
{
    uint8_t block_size;
    uint8_t application_identifier[8];
    uint8_t application_authentication_code[3];
    uint8_t sub_blocks_block_size;
};

struct gif_comment_extension_block_t
{
};

struct gif_graphic_control_extension_block_t
{
    uint8_t block_size;
    uint8_t packed_fields;
    uint16_t delay_time;
    uint8_t transparent_color_index;
    uint8_t block_terminator;
};

struct gif_extension_block_t
{
    uint16_t header;
    union
    {
        struct gif_application_extension_block_t application;
        struct gif_comment_extension_block_t comment;
        struct gif_graphic_control_extension_block_t graphic_control;
    };
};

bool gif_is_extension_block(struct gif_global_state_t *);
void gif_get_extension_block(struct gif_global_state_t *, struct gif_extension_block_t *);
void gif_get_extension_block_sub_blocks(struct gif_global_state_t *, struct gif_extension_block_t *, uint8_t *const _);