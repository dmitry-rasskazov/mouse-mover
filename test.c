#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include "mouse-mover.h"

int main()
{
    int status, valread, client_fd;
    struct sockaddr_un serv_addr;
    struct MoveCommand moveCommand;
    memset(&moveCommand, 0, sizeof(moveCommand));
    moveCommand.pos_x = 250;
    moveCommand.pos_y = 250;
    moveCommand.checksum = checksum_command(moveCommand);

    struct MoveResponse response;
    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, "/run/mouse-mover.sock");

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed: %s\n", strerror(errno));
        return -1;
    }

    send(client_fd, &moveCommand, sizeof(moveCommand), 0);

    valread = read(client_fd, &response, sizeof(response));
    printf("Read: %d bytes\n", valread);

    if(valread <= 0) {
        printf("Response reading failure by reading");
    }

    if(response.checksum != checksum_response(response)) {
        printf("Response reading failure by validation");
    }

    // closing the connected socket
    close(client_fd);
    return 0;
}