#include <stdint.h>
#include <stdbool.h>

struct gif_signature_t
{
    unsigned char signature[7];
};

struct gif_screen_descriptor_t
{
};

void gif_get_signature(const uint8_t *, struct gif_signature_t *);
uint16_t gif_screen_width(uint8_t *data);
uint16_t gif_screen_height(uint8_t *data);
bool gif_has_global_color_table_flag(uint8_t *data);
bool gif_has_sort_flag(uint8_t *data);
uint8_t gif_color_resolution_bits(uint8_t *data);
uint8_t gif_global_color_table_bits(uint8_t *data);
uint16_t gif_global_color_table_size(uint8_t global_color_table_bits);