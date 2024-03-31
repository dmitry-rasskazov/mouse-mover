#include <stdint.h>

struct moveCommand {
    int pos_x;
    int pos_y;
    uint8_t checksum;
};

struct moveResponse {
    int error_code;
    uint8_t checksum;
};

uint8_t checksum(void * array, int size);

uint8_t checksum(struct moveCommand * command)
{
    return checksum((void *) command, sizeof(*command));
}

uint8_t checksum(struct moveResponse * response)
{
    return checksum((void *) response, sizeof(*response));
}

uint8_t checksum(void * array, int size)
{
    size = size - 1;
    uint8_t * bytes = (uint8_t *) array;

    uint8_t CK_A = 0, CK_B = 0;
    for(uint8_t i = 0; i < size; i++) {
        CK_A = CK_A + bytes[i];
        CK_B = CK_B + CK_A;
    }

    return CK_B;
}