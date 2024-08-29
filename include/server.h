#ifndef SERVER_H
#define SERVER_H
#include "protocol.h"

#define MAX_GAMES 100

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

#endif //SERVER_H
