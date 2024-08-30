#include "server.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "utils.h"

Game *sessions[MAX_GAMES];
pthread_mutex_t sessions_mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    return 0;
    const fd_t server_fd = socket(AF_INET, SOCK_STREAM, 0); // use IPv4 and TCP
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8080),
        .sin_addr = INADDR_ANY
    };

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, MAX_CONNECTION_QUEUE) == -1) {
        perror("listen");
        return 1;
    }

    while (1) {
        const fd_t player_fd = accept(server_fd, NULL, NULL);
        if (player_fd == -1) {
            perror("accept");
            return 1;
        }

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, (void *) player_fd);
    }
}

int create_session(const fd_t player1_fd) {
}
void join_sessioin(const int session_id, const fd_t player2_fd) {
}

void process_move(int session_id, fd_t player_fd, int position) {
}

void end_session(int session_id) {
}

void handle_client(void *arg) {
    const fd_t player_fd = *(fd_t *) arg;
    int session_id = -1;
}
