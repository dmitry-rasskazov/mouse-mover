struct moveCommand {
    int pos_x;
    int pos_y;
    unsigned char checksum;
};

struct moveResponse {
    int error_code;
    unsigned char checksum;
};

unsigned char checksum_native(void *, unsigned int);

unsigned char checksum_command(struct moveCommand command)
{
    return checksum_native(&command, sizeof(command));
}

unsigned char checksum_response(struct moveResponse response)
{
    return checksum_native(&response, sizeof(response));
}

unsigned char checksum_native(void *array, unsigned int size)
{
    size = size - 1;
    unsigned char * bytes = (unsigned char *) array;

    unsigned char CK_A = 0, CK_B = 0;
    for(unsigned char i = 0; i < size; i++) {
        CK_A = (CK_A + bytes[i]) & 0xFF;
        CK_B = (CK_B + CK_A) & 0xFF;
    }

    return CK_B;
}
