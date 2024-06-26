#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <errno.h>
#include <string.h>
#include "mouse-mover.h"


Display *dpy;
Window root_window;
int started = 1;
int server_socket;

XEvent createXPointerEvent();
void moveMouse(int posX, int poxY);
int openSocket(struct sockaddr_un *);
void closeSocket(int);
void printError(char*);
void requestProcess(int, struct sockaddr_un);

void intHandler(int dummy)
{
    closeSocket(server_socket);
    exit(0);
}

int main()
{
    printf("Stat program\n");

    dpy = XOpenDisplay(NULL);
    root_window = XRootWindow(dpy, 0);
    signal(SIGINT, intHandler);

    printf("Registered SIGINT handler\n");

    struct sockaddr_un server_addr, client_addr;

    int ssd, csd;

    if((ssd = openSocket(&server_addr)) == -1) {
        printError("open socket");
        return 1;
    }

    printf("Opened socket\n");

    server_socket = ssd;

    listen(ssd, 1);

    while(started) {
        printf("Socket listened\n");
        socklen_t clen = sizeof(client_addr);
        csd = accept(ssd, (struct sockaddr *) &client_addr, &clen);

        printf("Accept successful\n");
        requestProcess(csd, client_addr);
    }

    closeSocket(ssd);
    XCloseDisplay( dpy );

    return 0;
}

int openSocket(struct sockaddr_un * server_addr)
{
    int server_socket;

    if((server_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        return -1;
    }

    server_addr->sun_family = AF_UNIX;
    strcpy(server_addr->sun_path, "/run/mouse-mover.sock");
    int slen = sizeof(*server_addr);

    if(bind(server_socket, (struct sockaddr *) server_addr, slen) == -1) {
        return -1;
    }

    return server_socket;
}

void closeSocket(int sd)
{
    if(close(sd) == -1) {
        printError("close socket");
    }

    unlink("/run/mouse-mover.sock");
}

void printError(char * context)
{
    printf("Failure %s. Message: %s\n", context, strerror(errno));
}

void requestProcess(int sd, struct sockaddr_un client_address)
{
    struct MoveCommand moveCommand;

    if(read(sd, &moveCommand, sizeof(moveCommand)) == -1) printError("read command");

    struct MoveResponse response;
    if(moveCommand.checksum != checksum_command(moveCommand)) {
        printf(
            "Incorrect command! Checksum invalid: %d != %d. X: %d, Y: %d\n",
            moveCommand.checksum,
            checksum_command(moveCommand),
            moveCommand.pos_x,
            moveCommand.pos_y
        );

        response.error_code = -1;

    } else {
        moveMouse(moveCommand.pos_x, moveCommand.pos_y);

        response.error_code = 0;
    }

    response.checksum = checksum_response(response);
    if(write(sd, &response, sizeof(response)) == -1) printError("write response");
}

void moveMouse(int posX, int poxY)
{
    XEvent event = createXPointerEvent();

    XSelectInput(dpy, root_window, KeyReleaseMask);
    XWarpPointer(dpy, None, root_window, 0, 0, event.xbutton.x, event.xbutton.y, posX, poxY);
    XFlush(dpy);
}

XEvent createXPointerEvent()
{
    XEvent event;

    XQueryPointer(
        dpy,
        root_window,
        &event.xbutton.root,
        &event.xbutton.window,
        &event.xbutton.x_root,
        &event.xbutton.y_root,
        &event.xbutton.x,
        &event.xbutton.y,
        &event.xbutton.state
    );

    return event;
}
