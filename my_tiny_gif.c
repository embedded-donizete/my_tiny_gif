#include <string.h>
#include <math.h>

#include <my_tiny_gif.h>

void gif_get_signature(const uint8_t *gif, struct gif_signature_t *ptr)
{
    memcpy(ptr, gif, 6);
    ptr->signature[6] = 0;
}

#define GIF_SCREEN_DESCRIPTOR_WIDTH_OFFSET 6

uint16_t gif_screen_width(uint8_t *data)
{
    uint16_t width = data[GIF_SCREEN_DESCRIPTOR_WIDTH_OFFSET];
    width |= data[GIF_SCREEN_DESCRIPTOR_WIDTH_OFFSET + 1] << 8;
    return width;
}

#define GIF_SCREEN_DESCRIPTOR_HEIGHT_OFFSET 8

uint16_t gif_screen_height(uint8_t *data)
{
    uint16_t height = data[GIF_SCREEN_DESCRIPTOR_HEIGHT_OFFSET];
    height |= data[GIF_SCREEN_DESCRIPTOR_HEIGHT_OFFSET + 1] << 8;
    return height;
}

#define GIF_SCREEN_PACKED_FIELDS_OFFSET 10

#define GIF_SCREEN_PACKED_FIELDS_M_MASK 0b10000000

bool gif_has_global_color_table_flag(uint8_t *data)
{
    return data[GIF_SCREEN_PACKED_FIELDS_OFFSET] & GIF_SCREEN_PACKED_FIELDS_M_MASK;
}

#define GIF_SCREEN_PACKED_FIELDS_SORT_MASK 0b00001000

bool gif_has_sort_flag(uint8_t *data)
{
    return data[GIF_SCREEN_PACKED_FIELDS_OFFSET] & GIF_SCREEN_PACKED_FIELDS_SORT_MASK;
}

#define GIF_SCREEN_PACKED_FIELDS_COLOR_RESOLUTION_MASK 0b01110000

uint8_t gif_color_resolution_bits(uint8_t *data)
{
    return ((data[GIF_SCREEN_PACKED_FIELDS_OFFSET] & GIF_SCREEN_PACKED_FIELDS_COLOR_RESOLUTION_MASK) >> 4) + 1;
}

#define GIF_SCREEN_PACKED_FIELDS_COLOR_TABLE_MASK 0b00000111

uint8_t gif_global_color_table_bits(uint8_t *data)
{
    return (data[GIF_SCREEN_PACKED_FIELDS_OFFSET] & GIF_SCREEN_PACKED_FIELDS_COLOR_TABLE_MASK) + 1;
}

uint16_t gif_global_color_table_size(uint8_t global_color_table_bits)
{
    return 3 * pow(2, global_color_table_bits);
}