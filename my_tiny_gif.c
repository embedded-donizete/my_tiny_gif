#include <string.h>

#include <my_tiny_gif.h>

#define HEADER "GIF89a "

bool gif_has_a_valid_header(uint8_t *data, uint64_t size)
{
    if (size < sizeof(HEADER))
        return false;
    return !memcmp(data, HEADER, sizeof(HEADER));
}