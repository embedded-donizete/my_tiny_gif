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
    gif_application_extension = 0xFF21
};

struct gif_extension {
    uint8_t introducer;
    uint8_t label;
};

struct application_extension {
    struct gif_extension header;
};

struct comment_extension {

};

struct gif_global_state_t
{
    uint8_t *global_color_map_memory;
    uint16_t global_color_map_size;
};

void gif_get_header(const uint8_t *const _, struct gif_header_t *);
void gif_get_logical_screen_descriptor(const u_int8_t *const _, struct gif_logical_screen_descriptor_t *);
uint16_t gif_get_global_color_table_size(const struct gif_logical_screen_descriptor_t *);