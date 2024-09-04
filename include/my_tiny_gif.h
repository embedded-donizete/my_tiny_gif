#include <stdint.h>
#include <stdbool.h>

bool gif_has_a_valid_header(uint8_t *data);
void gif_header_version(uint8_t *data, uint8_t *version);