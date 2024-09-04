#include <stdio.h>
#include <stdlib.h>

#include <my_tiny_gif.h>

int main(int argc, char const *argv[])
{
    FILE *file = fopen("../sample/homer.gif", "rb");
    if (!file)
    {
        fprintf(stderr, "File can't be open");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    uint64_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    uint8_t buff[size];
    fread(buff, size, size, file);

    if (!gif_has_a_valid_header(buff, size))
    {
        fprintf(stderr, "Is it a GIF89a?");
        goto exit;
    }

exit:
    fclose(file);

    return 0;
}
