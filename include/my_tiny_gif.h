#include <stdint.h>
#include <stdbool.h>

struct gif_signature_t
{
    unsigned char buffer[8];
};

struct gif_screen_descriptor_t
{
    uint16_t width;
    uint16_t height;
    uint8_t packed_fields;
    uint8_t background_color_index;
    uint8_t pixel_aspect_ratio;
};

void gif_get_signature(const uint8_t *, struct gif_signature_t *);
void gif_get_screen_descriptor(const u_int8_t *, struct gif_screen_descriptor_t *);
uint16_t gif_get_global_color_table_size(const struct gif_screen_descriptor_t *);
void gif_get_global_color_table(const u_int8_t *, u_int8_t *, uint16_t);