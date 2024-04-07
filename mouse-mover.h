#include <stdarg.h>

struct MoveCommand {
    int pos_x;
    int pos_y;
    unsigned char checksum;
};

struct MoveResponse {
    int error_code;
    unsigned char checksum;
};

unsigned char checksum_native(unsigned int size, ...);

unsigned char checksum_command(struct MoveCommand command)
{
    return checksum_native(2, command.pos_x, command.pos_y);
}

unsigned char checksum_response(struct MoveResponse response)
{
    return checksum_native(1, response.error_code);
}

unsigned char checksum_native(unsigned int size, ...)
{
    va_list factor;
    va_start(factor, size);

    unsigned char CK_A = 0, CK_B = 0;
    for(unsigned char i = 0; i < size; i++) {
        CK_A = (CK_A + va_arg(factor, int)) & 0xFF;
        CK_B = (CK_B + CK_A) & 0xFF;
    }

    va_end(factor);
    return CK_B;
}
