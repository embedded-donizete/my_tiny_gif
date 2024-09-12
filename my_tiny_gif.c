#include <string.h>
#include <math.h>

#include <my_tiny_gif.h>

void gif_init_global_state(const uint8_t *const gif, struct gif_global_state_t *ptr)
{
    memset(ptr, 0, sizeof(*ptr));
    ptr->gif_pointer = gif;

    memcpy(&ptr->header, ptr->gif_pointer, sizeof(ptr->header));
    ptr->gif_pointer += sizeof(ptr->header);

    memcpy(&ptr->logical_screen_descriptor, ptr->gif_pointer, sizeof(ptr->logical_screen_descriptor));
    ptr->gif_pointer += sizeof(ptr->logical_screen_descriptor);
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

void gif_init_global_state_color_map(struct gif_global_state_t *ptr, uint16_t size, uint8_t *buffer)
{
    ptr->color_map_buffer = buffer;
    ptr->color_map_size = size;

    memcpy(buffer, ptr->gif_pointer, size);
    ptr->gif_pointer += size;
}

bool gif_is_extension_block(struct gif_global_state_t *ptr)
{
    switch (*(uint16_t *)(ptr->gif_pointer))
    {
    case gif_application_extension_label:
    case gif_comment_extension_label:
    case gif_graphic_control_extension_label:
        return true;
    default:
        return false;
    }
}

void gif_get_extension_block(
    struct gif_global_state_t *state,
    struct gif_extension_block_t *block)
{
    memcpy(block, state->gif_pointer, sizeof(*block));
    state->gif_pointer += sizeof(block->header);

    switch (block->header)
    {
    case gif_application_extension_label:
        state->gif_pointer += sizeof(block->application);
        break;
    case gif_comment_extension_label:
        state->gif_pointer += sizeof(block->comment);
        break;
    case gif_graphic_control_extension_label:
        state->gif_pointer += sizeof(block->graphic_control);
        break;
    }
}

void gif_get_extension_block_sub_blocks(
    struct gif_global_state_t *state,
    struct gif_extension_block_t *block,
    uint8_t *const sub_blocks)
{
    switch (block->header)
    {
    case gif_application_extension_label:
    {
        uint8_t sub_blocks_block_size = block->application.sub_blocks_block_size;

        memcpy(sub_blocks, state->gif_pointer, sub_blocks_block_size);
        state->gif_pointer += sub_blocks_block_size;

        memcpy(&block->application.sub_blocks_block_size, state->gif_pointer, sizeof(uint8_t));
        state->gif_pointer += sizeof(uint8_t);

        break;
    }
    case gif_comment_extension_label:
        break;
    }
}
