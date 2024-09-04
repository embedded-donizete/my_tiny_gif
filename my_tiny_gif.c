#include <string.h>
#include <math.h>

#include <my_tiny_gif.h>

void gif_get_signature(const uint8_t *gif, struct gif_signature_t *ptr)
{
    memcpy(ptr, gif, 6);
    ptr->buffer[6] = 0;
}

#define GIF_SCREEN_DESCRIPTOR_WIDTH_OFFSET 6
#define GIF_SCREEN_DESCRIPTOR_HEIGHT_OFFSET 8
#define GIF_SCREEN_PACKED_FIELDS_OFFSET 10

#define GIF_SCREEN_PACKED_FIELDS_M_MASK 0b10000000
#define GIF_SCREEN_PACKED_FIELDS_COLOR_RESOLUTION_MASK 0b01110000
#define GIF_SCREEN_PACKED_FIELDS_SORT_MASK 0b00001000
#define GIF_SCREEN_PACKED_FIELDS_COLOR_TABLE_MASK 0b00000111

void gif_get_screen_descriptor(const u_int8_t *gif, struct gif_screen_descriptor_t *ptr)
{
    ptr->width = gif[GIF_SCREEN_DESCRIPTOR_WIDTH_OFFSET];
    ptr->width |= gif[GIF_SCREEN_DESCRIPTOR_WIDTH_OFFSET + 1] << 8;

    ptr->height = gif[GIF_SCREEN_DESCRIPTOR_HEIGHT_OFFSET];
    ptr->height |= gif[GIF_SCREEN_DESCRIPTOR_HEIGHT_OFFSET + 1] << 8;

    ptr->packed_fields = gif[GIF_SCREEN_PACKED_FIELDS_OFFSET];
}

uint16_t gif_get_global_color_table_size(const struct gif_screen_descriptor_t *ptr)
{
    if (!(ptr->packed_fields & GIF_SCREEN_PACKED_FIELDS_M_MASK))
        return 0;
    uint8_t bits_count = (ptr->packed_fields & GIF_SCREEN_PACKED_FIELDS_COLOR_TABLE_MASK) + 1;
    return 3 * pow(2, bits_count);
}

#define GIF_GLOBAL_COLOR_MAP_OFFSET 13

void gif_get_global_color_table(const u_int8_t *gif, const u_int8_t *buffer, uint16_t size)
{
    memcpy(buffer, gif + GIF_GLOBAL_COLOR_MAP_OFFSET, size);
}