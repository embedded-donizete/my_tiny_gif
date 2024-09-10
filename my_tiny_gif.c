#include <string.h>
#include <math.h>

#include <my_tiny_gif.h>

void gif_get_header(const uint8_t *gif, struct gif_header_t *ptr)
{
    memcpy(ptr, gif, sizeof(*ptr));
}

void gif_get_logical_screen_descriptor(const u_int8_t *const gif, struct gif_logical_screen_descriptor_t *ptr)
{
    memcpy(ptr, gif + sizeof(struct gif_header_t), sizeof(*ptr));
}

uint16_t gif_get_global_color_table_size(const struct gif_logical_screen_descriptor_t *ptr)
{
    if (ptr->packed_fields & global_map_mask) {
        uint8_t bits_count = ptr->packed_fields & color_table_mask;
        uint16_t possibilities = 2 << bits_count;
        return 3 * possibilities;
    }
    return 0;
}

void gif_get_global_state(const uint8_t *const gif, struct gif_global_state_t *ptr)
{
    uint16_t offset = 0;
    offset += sizeof(struct gif_header_t);
    offset += sizeof(struct gif_logical_screen_descriptor_t);

    if (ptr->color_map_size) {
        memcpy(ptr->color_map_memory, gif + offset, ptr->color_map_size);
        offset += ptr->color_map_size;
    }

    ptr->static_offset = offset;
    ptr->dynamic_offset = offset;
}