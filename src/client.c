#include "client.h"
#include <stdio.h>

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
}
