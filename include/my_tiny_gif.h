#include <stdint.h>
#include <stdbool.h>

struct gif_signature_t
{
    unsigned char buffer[7];
};

struct gif_screen_descriptor_t
{
    uint16_t width;
    uint16_t height;
    uint8_t packed_fields;
    uint8_t background_color_index;
    uint8_t pixel_aspect_ratio;
};

enum packet_fields_mask {
    global_map_mask         = 0b10000000,
    color_resolution_mask   = 0b01110000,
    sort_mask_mask          = 0b00001000,
    color_table_mask        = 0b00000111,
};

struct gif_state_t
{
    uint8_t *global_color_map_memory;
    uint16_t global_color_map_size;
    const uint8_t *first_frame_pointer;
    uint16_t *current_frame_pointer;
};

void gif_get_signature(const uint8_t *const _, struct gif_signature_t *);
void gif_get_screen_descriptor(const u_int8_t *const _, struct gif_screen_descriptor_t *);
uint16_t gif_get_global_color_table_size(const struct gif_screen_descriptor_t *);
void gif_start_state(const u_int8_t *const _, struct gif_state_t *);