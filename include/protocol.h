#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef int fd_t;

typedef enum {
    EMPTY = ' ',
    PLAYER_1 = 'X',
    PLAYER_2 = 'O'
} Tile;

typedef Tile Board[9];

#endif //PROTOCOL_H
