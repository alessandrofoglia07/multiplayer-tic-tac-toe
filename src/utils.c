#include "utils.h"

#include <stdio.h>

int check_winner(const Board board) {
    if (board[0] == board[1] && board[1] == board[2] && board[0] != EMPTY) {
        return board[0];
    }
    if (board[0] == board[3] && board[3] == board[6] && board[0] != EMPTY) {
        return board[0];
    }
    if (board[0] == board[4] && board[4] == board[8] && board[0] != EMPTY) {
        return board[0];
    }
    if (board[1] == board[4] && board[4] == board[7] && board[1] != EMPTY) {
        return board[1];
    }
    if (board[2] == board[5] && board[5] == board[8] && board[2] != EMPTY) {
        return board[2];
    }
    if (board[2] == board[4] && board[4] == board[6] && board[2] != EMPTY) {
        return board[2];
    }
    if (board[3] == board[4] && board[4] == board[5] && board[3] != EMPTY) {
        return board[3];
    }
    if (board[6] == board[7] && board[7] == board[8] && board[6] != EMPTY) {
        return board[6];
    }
    return EMPTY;
}

int is_board_full(const Board board) {
    for (int i = 0; i < 9; i++) {
        if (board[i] == EMPTY) {
            return 0;
        }
    }
    return 1;
}

void print_board(const Board board) {
    printf("    |   |    \n");
    printf("  %c | %c | %c  \n", board[0], board[1], board[2]);
    printf("____|___|____\n");
    printf("    |   |   \n");
    printf("  %c | %c | %c  \n", board[3], board[4], board[5]);
    printf("____|___|____\n");
    printf("    |   |    \n");
    printf("  %c | %c | %c  \n", board[6], board[7], board[8]);
    printf("    |   |    \n");
}
