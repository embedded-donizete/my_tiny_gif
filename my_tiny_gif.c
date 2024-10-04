#include <string.h>
#include <math.h>

#include <my_tiny_gif.h>

#define memcpy_and_inc_state(dst, size)    \
    memcpy(dst, state->gif_pointer, size); \
    state->gif_pointer += size;

void gif_init_global_state(const uint8_t *const gif, struct gif_global_state_t *state)
{
    state->gif_pointer = gif;
    memcpy_and_inc_state(&state->header, sizeof(state->header));
    memcpy_and_inc_state(&state->logical_screen_descriptor, sizeof(state->logical_screen_descriptor));
}

uint16_t gif_get_global_color_table_size(const struct gif_global_state_t *ptr)
{
    if (ptr->logical_screen_descriptor.packed_fields & global_map_mask)
    {
        uint8_t bits_count = ptr->logical_screen_descriptor.packed_fields & color_table_mask;
        uint16_t possibilities = 2 << bits_count;
        return 3 * possibilities;
    }
    return 0;
}

void gif_init_global_state_color_map(struct gif_global_state_t *state, uint8_t *buffer, uint16_t size)
{
    state->color_map_buffer = buffer;
    state->color_map_size = size;

    memcpy_and_inc_state(buffer, size);
}

uint16_t gif_is_extension_block(struct gif_global_state_t *ptr)
{
    uint16_t extension_block_label = *(uint16_t *)(ptr->gif_pointer);
    switch (extension_block_label)
    {
    case gif_application_extension_label:
    case gif_comment_extension_label:
    case gif_graphic_control_extension_label:
        ptr->gif_pointer += sizeof(uint16_t);
        return extension_block_label;
    default:
        return 0;
    }
}

void gif_get_extension_block(
    struct gif_global_state_t *state,
    void *dst)
{
    uint8_t block_size;
    memcpy_and_inc_state(&block_size, sizeof(uint8_t));
    memcpy_and_inc_state(dst, block_size);
}

uint8_t gif_get_extension_block_sub_blocks_size(struct gif_global_state_t *state)
{
    uint8_t size;
    memcpy_and_inc_state(&size, sizeof(uint8_t));
    return size;
}

void gif_get_extension_block_sub_blocks(
    struct gif_global_state_t *state,
    uint8_t data_size,
    uint8_t *const sub_blocks)
{
    memcpy_and_inc_state(sub_blocks, data_size);
}

void gif_get_image_descriptor(
    struct gif_global_state_t *state,
    struct gif_image_descriptor_t *image_descriptor)
{
    memcpy_and_inc_state(image_descriptor, sizeof(*image_descriptor));
}
