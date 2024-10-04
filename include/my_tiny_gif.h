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
void gif_init_global_state_color_map(struct gif_global_state_t *, uint8_t *, uint16_t);

enum gif_extension_block_enum
{
    gif_application_extension_label = 0xFF21,
    gif_comment_extension_label = 0xFE21,
    gif_graphic_control_extension_label = 0xF921
};

struct gif_application_extension_block_t
{
    uint8_t application_identifier[8];
    uint8_t application_authentication_code[3];
};

struct gif_comment_extension_block_t
{
};

struct gif_graphic_control_extension_block_t
{
    uint8_t packed_fields;
    uint16_t delay_time;
    uint8_t transparent_color_index;
};

uint16_t gif_is_extension_block(struct gif_global_state_t *);
void gif_get_extension_block(struct gif_global_state_t *, void *);
uint8_t gif_get_extension_block_sub_blocks_size(struct gif_global_state_t *);
void gif_get_extension_block_sub_blocks(struct gif_global_state_t *, uint8_t, uint8_t *const _);

struct gif_image_descriptor_t {
    uint8_t separator;
    uint16_t left_position;
    uint16_t right_position;
    uint16_t width;
    uint16_t height;
    uint8_t packed_fields;
} __attribute__((packed));

void gif_get_image_descriptor(struct gif_global_state_t*, struct gif_image_descriptor_t*);