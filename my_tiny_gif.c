#include <string.h>

#include <my_tiny_gif.h>

static uint8_t HEADER[] = {'G', 'I', 'F'};

bool gif_has_a_valid_header(uint8_t *data)
{
    return !memcmp(data, HEADER, sizeof(HEADER));
}

void gif_header_version(uint8_t *data, uint8_t *version)
{
    memcpy(version, data + sizeof(HEADER), 3);
}