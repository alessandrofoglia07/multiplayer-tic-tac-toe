#include "protocol.h"

#include <string.h>
#include <sys/socket.h>

typedef int fd_t;

int send_message(const fd_t fd, const Message *message) {
    char buf[sizeof(Message)];
    memcpy(buf, message, sizeof(Message));
    return send(fd, buf, sizeof(Message), 0);
}

int receive_message(const fd_t fd, Message *message) {
    char buf[sizeof(Message)];
    const int bytes_received = recv(fd, buf, sizeof(Message), 0);
    if (bytes_received == sizeof(Message)) {
        memcpy(message, buf, sizeof(Message));;
    }
    return bytes_received;
}
