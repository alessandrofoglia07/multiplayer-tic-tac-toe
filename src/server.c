#include "server.h"
#include "utils.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

Game *sessions[MAX_GAMES];
pthread_mutex_t sessions_mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    const fd_t server_fd = socket(AF_INET, SOCK_STREAM, 0); // use IPv4 and TCP
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT),
        .sin_addr = {
            .s_addr = inet_addr(SERVER_IP)
        }
    };

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1) {
        perror("setsockopt");
        return 1;
    }

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

        fd_t *player_fd_ptr = (fd_t *) malloc(sizeof(fd_t));
        if (player_fd_ptr == NULL) {
            perror("malloc");
            close(player_fd);
            continue;
        }
        *player_fd_ptr = player_fd;

        pthread_t client_thread;
        const int err = pthread_create(&client_thread, NULL, handle_client, player_fd_ptr);
        if (err != 0) {
            perror("pthread_create");
            close(player_fd);
            free(player_fd_ptr);
            return 1;
        }

        pthread_detach(client_thread);
    }

    close(server_fd);
    return 0;
}

int create_session(const fd_t player1_fd) {
    pthread_mutex_lock(&sessions_mutex);
    for (int i = 0; i < MAX_GAMES; i++) {
        if (sessions[i] == NULL) {
            sessions[i] = (Game *) malloc(sizeof(Game));
            if (sessions[i] == NULL) {
                perror("malloc");
                pthread_mutex_unlock(&sessions_mutex);
                return -1;
            }
            memcpy(sessions[i], &(Game){0}, sizeof(Game));
            sessions[i]->player1.socket = player1_fd;
            sessions[i]->player1.character = 'X';
            sessions[i]->player2.socket = 0;
            sessions[i]->player2.character = 'O';
            sessions[i]->turn = Player1; // Player 1 starts
            sessions[i]->is_active = 1;
            const Board EMPTY_BOARD = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
            memcpy(sessions[i]->board, EMPTY_BOARD, sizeof(sessions[i]->board));
            pthread_mutex_unlock(&sessions_mutex);
            return i; // return session id
        }
    }
    pthread_mutex_unlock(&sessions_mutex);
    return -1; // no available slot for new session
}

void join_session(const int session_id, const fd_t player2_fd) {
    pthread_mutex_lock(&sessions_mutex);
    sessions[session_id]->player2.socket = player2_fd;
    Message msg;
    strcpy(msg.type, MSG_GAME_START);
    memcpy(msg.board, sessions[session_id]->board, sizeof(msg.board));
    msg.character = sessions[session_id]->player1.character;
    send_message(sessions[session_id]->player2.socket, &msg);
    msg.character = sessions[session_id]->player2.character;
    send_message(sessions[session_id]->player1.socket, &msg);
    pthread_mutex_unlock(&sessions_mutex);
}

void process_move(const int session_id, const fd_t player_fd, const int position) {
    pthread_mutex_lock(&sessions_mutex);
    Game *session = sessions[session_id];

    if (session->is_active && session->board[position] == EMPTY &&
        ((session->turn == Player1 && player_fd == session->player1.socket) ||
         (session->turn == Player2 && player_fd == session->player2.socket))) {
        session->board[position] = session->turn == Player1 ? session->player1.character : session->player2.character;

        // check if the game is over
        const char winner = check_winner(session->board);
        if (winner != EMPTY || is_board_full(session->board)) {
            session->is_active = 0;
            Message msg;
            strcpy(msg.type, MSG_GAME_OVER);
            msg.character = winner; // winner or EMPTY if it's a draw
            send_message(session->player1.socket, &msg);
            send_message(session->player2.socket, &msg);
        } else {
            // switch turns
            session->turn = session->turn == Player1 ? Player2 : Player1;
        }

        // send updated state to both players
        Message msg;
        strcpy(msg.type, MSG_STATE_UPDATE);
        memcpy(msg.board, session->board, sizeof(msg.board));
        send_message(session->player1.socket, &msg);
        send_message(session->player2.socket, &msg);
    }

    pthread_mutex_unlock(&sessions_mutex);
}

void end_session(const int session_id) {
    pthread_mutex_lock(&sessions_mutex);
    free(sessions[session_id]);
    sessions[session_id] = NULL;
    pthread_mutex_unlock(&sessions_mutex);
}

void *handle_client(void *arg) {
    const fd_t player_fd = *(fd_t *) arg;
    free(arg); // Free the malloc'd memory for player_fd_ptr

    int session_id = -1;

    // check if there's an available session or create a new one
    pthread_mutex_lock(&sessions_mutex);
    for (int i = 0; i < MAX_GAMES; i++) {
        if (sessions[i] != NULL && sessions[i]->player2.socket == 0) {
            join_session(i, player_fd); // join existing session
            session_id = i;
            break;
        }
    }
    if (session_id == -1) {
        session_id = create_session(player_fd); // create new session
    }
    pthread_mutex_unlock(&sessions_mutex);

    if (session_id == -1) {
        close(player_fd);
        return NULL;
    }

    while (sessions[session_id]->is_active) {
        Message msg;
        // wait for player's move
        const int read_size = receive_message(player_fd, &msg);
        if (read_size > 0 && strcmp(msg.type, MSG_MOVE) == 0) {
            // parse move
            for (int i = 0; i < 9; i++) {
                if (sessions[session_id]->board[i] != msg.board[i]) {
                    process_move(session_id, player_fd, i);
                    break;
                }
            }
        } else {
            break;
        }
    }

    end_session(session_id);

    close(player_fd);
    pthread_exit(NULL);
}
