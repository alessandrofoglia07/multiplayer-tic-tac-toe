#ifndef UTILS_H
#define UTILS_H
#include "protocol.h"

/**
 * @return the winner of the game, or -1 if there is no winner
 */
int check_winner(const Board board);

/**
 * @return 1 if the board is full, 0 otherwise
 */
int is_board_full(const Board board);

void print_board(const Board board);

#endif //UTILS_H
