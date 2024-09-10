#include <stdint.h>
#include <stdbool.h>

struct gif_header_t
{
    uint8_t signature   [3];
    uint8_t version     [3];
} __attribute__((packed));

enum gif_logical_screen_descriptor_packed_fields_bitwise_enum {
    global_map_mask         = 0b10000000,
    color_resolution_mask   = 0b01110000,
    sort_mask_mask          = 0b00001000,
    color_table_mask        = 0b00000111,
};

struct gif_logical_screen_descriptor_t
{
    uint16_t width;
    uint16_t height;
    uint8_t packed_fields;
    uint8_t background_color_index;
    uint8_t pixel_aspect_ratio;
} __attribute__((packed));

enum gif_extension_enum {
    gif_application_extension = 0xFF21,
    gif_comment_extension = 0xFE21
};

struct application_extension_t {
    uint8_t block_size;
    uint8_t application_identifier[8];
    uint8_t application_authentication_code[3];
    uint8_t sub_blocks_block_size;
};

struct comment_extension_t {
};

struct special_purpose_block_t {
    uint16_t header;
    union {
        struct application_extension_t application_extension;
        struct comment_extension_t comment_extension;
    };
};

struct gif_global_state_t
{
    uint8_t *color_map_memory;
    uint16_t color_map_size;
    // where data starts after header and [global map]
    uint16_t static_offset;
    // where our current iteration is pointing to
    uint16_t dynamic_offset;
};

void gif_get_header(const uint8_t *const _, struct gif_header_t *);
void gif_get_logical_screen_descriptor(const u_int8_t *const _, struct gif_logical_screen_descriptor_t *);
uint16_t gif_get_global_color_table_size(const struct gif_logical_screen_descriptor_t *);
void gif_get_global_state(const uint8_t *const _, struct gif_global_state_t*);
bool gif_is_special_purpose_block(const uint8_t *const _, struct gif_global_state_t*);
void gif_get_special_purpose_block(const uint8_t *const _, struct gif_global_state_t*, struct special_purpose_block_t*);
void gif_get_special_purpose_block_sub_blocks(const uint8_t *const _, struct gif_global_state_t*, struct special_purpose_block_t*, uint8_t *const __);