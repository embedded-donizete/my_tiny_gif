#include <string.h>
#include <math.h>

#include <my_tiny_gif.h>

void gif_get_header(const uint8_t *gif, struct gif_header_t *ptr)
{
    memset(ptr, 0, sizeof(*ptr));
    memcpy(ptr->signature, gif, gif_header_signature_size);
    memcpy(ptr->version, gif + gif_header_signature_size, gif_header_version_size);
}

void gif_get_logical_screen_descriptor(const u_int8_t *const gif, struct gif_logical_screen_descriptor_t *ptr)
{
    memcpy(ptr, gif + gif_header_total_size, sizeof(*ptr));
}

uint16_t gif_get_global_color_table_size(const struct gif_logical_screen_descriptor_t *ptr)
{
    if (!(ptr->packed_fields & global_map_mask)) return 0;
    uint8_t bits_count = (ptr->packed_fields & color_table_mask);
    uint16_t possibilities = 2 << bits_count;
    return 3 * possibilities;
}

#define GIF_GLOBAL_COLOR_MAP_OFFSET 13

void gif_start_state(const u_int8_t *const gif, struct gif_state_t *ptr)
{
    memcpy(ptr->global_color_map_memory, gif + GIF_GLOBAL_COLOR_MAP_OFFSET, ptr->global_color_map_size);
}