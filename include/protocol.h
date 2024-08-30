#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MSG_STATE_UPDATE "STATE_UPDATE"
#define MSG_GAME_OVER "GAME_OVER"

typedef int fd_t;

typedef enum {
    EMPTY = ' ',
    PLAYER_1 = 'X',
    PLAYER_2 = 'O'
} Tile;

typedef Tile Board[9];

typedef struct {
    char type[13];
    Board board;
} Message;

int send_message(const fd_t fd, const Message *message);

int receive_message(const fd_t fd, Message *message);

#endif //PROTOCOL_H
