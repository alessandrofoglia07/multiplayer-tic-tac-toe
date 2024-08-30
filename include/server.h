#ifndef SERVER_H
#define SERVER_H
#include "protocol.h"

#define MAX_GAMES 100
#define MAX_CONNECTION_QUEUE 10

enum Turn {
    Player1 = 1,
    Player2 = 2
};

typedef struct {
    fd_t socket;
    char character;
} Player;

typedef struct {
    Board board;
    Player player1;
    Player player2;
    enum Turn turn;
    int is_active;
} Game;

int create_session(fd_t player1_fd);

void join_sessioin(int session_id, fd_t player2_fd);

void process_move(int session_id, fd_t player_fd, int position);

void end_session(int session_id);

void handle_client(void *arg);

#endif //SERVER_H
