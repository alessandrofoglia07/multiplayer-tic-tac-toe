#ifndef SERVER_H
#define SERVER_H
#include "protocol.h"

#define MAX_GAMES 100

enum Turn {
    Player1 = 1,
    Player2 = 2
};

typedef struct {
    int board[3][3];
    fd_t player1;
    fd_t player2;
    enum Turn turn;
    int is_active;
} Game;

#endif //SERVER_H
