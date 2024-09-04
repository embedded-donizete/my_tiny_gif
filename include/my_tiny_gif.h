#include <stdint.h>
#include <stdbool.h>

bool gif_has_a_valid_header(uint8_t *data);
void gif_signature_version(uint8_t *data, uint8_t *version);
uint16_t gif_screen_width(uint8_t *data);
uint16_t gif_screen_height(uint8_t *data);
bool gif_has_global_color_map_flag(uint8_t *data);
bool gif_has_sort_flag(uint8_t *data);