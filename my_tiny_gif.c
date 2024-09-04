#include <string.h>

#include <my_tiny_gif.h>

static uint8_t HEADER[] = {'G', 'I', 'F'};

bool gif_has_a_valid_header(uint8_t *data)
{
    return !memcmp(data, HEADER, sizeof(HEADER));
}

#define GIF_SIGNATURE_VERSION_SIZE 3

void gif_signature_version(uint8_t *data, uint8_t *version)
{
    memcpy(version, data + sizeof(HEADER), GIF_SIGNATURE_VERSION_SIZE);
}

#define GIF_SCREEN_DESCRIPTOR_WIDTH_OFFSET 6

uint16_t gif_screen_descriptor_width(uint8_t *data)
{
    uint16_t width = data[GIF_SCREEN_DESCRIPTOR_WIDTH_OFFSET];
    width |= data[GIF_SCREEN_DESCRIPTOR_WIDTH_OFFSET + 1] << 8;
    return width;
}

#define GIF_SCREEN_DESCRIPTOR_HEIGHT_OFFSET 8

uint16_t gif_screen_descriptor_height(uint8_t *data)
{
    uint16_t height = data[GIF_SCREEN_DESCRIPTOR_HEIGHT_OFFSET];
    height |= data[GIF_SCREEN_DESCRIPTOR_HEIGHT_OFFSET + 1] << 8;
    return height;
}
