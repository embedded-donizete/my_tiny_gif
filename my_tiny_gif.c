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
    if (ptr->packed_fields & global_map_mask)
    {
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

    if (ptr->color_map_size)
    {
        memcpy(ptr->color_map_memory, gif + offset, ptr->color_map_size);
        offset += ptr->color_map_size;
    }

    ptr->static_offset = offset;
    ptr->dynamic_offset = offset;
}

bool gif_is_special_purpose_block(const uint8_t *const gif, struct gif_global_state_t *ptr)
{
    uint16_t next;
    memcpy(&next, gif + ptr->dynamic_offset, sizeof(next));
    return next == gif_application_extension || next == gif_comment_extension;
}

// how would be cool to have a new language
// with better support for algebraic types
#define when_special_block_is(          \
    block, exp1)                        \
    switch (block->header)              \
    {                                   \
    case gif_application_extension:     \
        exp1;                           \
        break;                          \
    }

void gif_get_special_purpose_block(
    const uint8_t *const gif,
    struct gif_global_state_t *state,
    struct special_purpose_block_t *block)
{
    memcpy(block, gif + state->dynamic_offset, sizeof(*block));

    state->dynamic_offset += sizeof(block->header);

    when_special_block_is(
        block,
        /* gif_application_extension -> */ {
            state->dynamic_offset += sizeof(block->application_extension);
        }
    );
}

void gif_get_special_purpose_block_sub_blocks(
    const uint8_t *const gif,
    struct gif_global_state_t *state,
    struct special_purpose_block_t *block,
    uint8_t *const sub_blocks)
{
    when_special_block_is(
        block,
        /* gif_application_extension -> */ {
            uint8_t sub_blocks_block_size = block->application_extension.sub_blocks_block_size;

            memcpy(sub_blocks, gif + state->dynamic_offset, sub_blocks_block_size);
            state->dynamic_offset += sub_blocks_block_size;

            memcpy(&block->application_extension.sub_blocks_block_size, gif + state->dynamic_offset, sizeof(uint8_t));
            state->dynamic_offset += sizeof(uint8_t);
        }
    );
}
