#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "protocol.h"
#include "utils.h"

int main() {
    printf("**** Tic Tac Toe ****\n");

    while (1) {
        printf("Select the game mode:\n");
        printf("-  Singleplayer (against computer)    [1]\n");
        printf("-  Multiplayer  (against real player) [2]\n");

        const GameMode mode = getchar();
        getchar(); // Consume the newline character

        if (mode == PLAYER_VS_PLAYER) {
            printf("Mode selected: Player vs Player\n");
            play_player_vs_player();
            break;
        }
        if (mode == PLAYER_VS_COMPUTER) {
            printf("Mode selected: Player vs Computer\n");
            play_player_vs_computer();
            break;
        }
        printf("Invalid mode selected\n");
    }

    return 0;
}

void play_player_vs_player() {
}

void play_player_vs_computer() {
    srand(time(NULL));

    char player;

    while (1) {
        printf("Enter the character you want to play with (X or O): ");
        player = getchar();
        getchar(); // Consume the newline character
        if (player == 'X' || player == 'O') {
            printf("Player selected: %c\n", player);
            break;
        }
        printf("Invalid player selected\n");
    }

    const char computer = player == 'X' ? 'O' : 'X';

    Board board = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
    int turn = 0;

    printf("The game has started\n");

    while (1) {
        print_board(board);

        // Player's turn
        while (1) {
            printf("Enter the position you want to play (1-9): ");
            const char position = getchar();
            getchar(); // Consume the newline character
            if (atoi(&position) >= 1 && atoi(&position) <= 9) {
                if (board[atoi(&position) - 1] == EMPTY) {
                    board[atoi(&position) - 1] = player;
                    break;
                }
                printf("Invalid position selected\n");
            }
        }
        if (check_winner(board) == player) {
            print_board(board);
            printf("Player wins!\n");
            break;
        }
        turn++;

        int computer_position = rand() % 9;
        while (board[computer_position] != EMPTY) {
            computer_position = rand() % 9;
        }
        board[computer_position] = computer;
        if (check_winner(board) == computer) {
            print_board(board);
            printf("Computer wins!\n");
            break;
        }
        turn++;

        if (is_board_full(board)) {
            printf("It's a tie!\n");
            break;
        }
    }
}
