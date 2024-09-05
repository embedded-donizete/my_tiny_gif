#include <string.h>
#include <math.h>

#include <my_tiny_gif.h>

void gif_get_signature(const uint8_t *gif, struct gif_signature_t *ptr)
{
    memcpy(ptr, gif, sizeof(struct gif_signature_t));
    ptr->buffer[sizeof(struct gif_signature_t) - 1] = 0;
}

#define GIF_SCREEN_DESCRIPTOR_OFFSET 6

#define GIF_SCREEN_PACKED_FIELDS_M_MASK 0b10000000
#define GIF_SCREEN_PACKED_FIELDS_COLOR_RESOLUTION_MASK 0b01110000
#define GIF_SCREEN_PACKED_FIELDS_SORT_MASK 0b00001000
#define GIF_SCREEN_PACKED_FIELDS_COLOR_TABLE_MASK 0b00000111

void gif_get_screen_descriptor(const u_int8_t *gif, struct gif_screen_descriptor_t *ptr)
{
    memcpy(ptr, gif + GIF_SCREEN_DESCRIPTOR_OFFSET, sizeof(struct gif_screen_descriptor_t));
}

uint16_t gif_get_global_color_table_size(const struct gif_screen_descriptor_t *ptr)
{
    if (!(ptr->packed_fields & GIF_SCREEN_PACKED_FIELDS_M_MASK))
        return 0;
    uint8_t bits_count = (ptr->packed_fields & GIF_SCREEN_PACKED_FIELDS_COLOR_TABLE_MASK) + 1;
    uint16_t possibilities = pow(2, bits_count);
    return 3 * possibilities;
}

#define GIF_GLOBAL_COLOR_MAP_OFFSET 13

void gif_start_state(const u_int8_t *const gif, struct gif_state_t *ptr)
{
    memcpy(ptr->global_color_map_memory, gif + GIF_GLOBAL_COLOR_MAP_OFFSET, ptr->global_color_map_size);
}