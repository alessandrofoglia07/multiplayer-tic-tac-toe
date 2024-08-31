#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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
    char player;
    Board board = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
    const fd_t sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT),
        .sin_addr.s_addr = inet_addr(SERVER_IP)
    };

    printf("Looking for an opponent...\n");
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to connect to the server\n");
        perror("connect");
        exit(1);
    }
    // wait for "GAME_START" message
    Message msg;
    receive_message(sockfd, &msg);
    if (strcmp(msg.type, MSG_GAME_START) == 0) {
        printf("Game found! Starting match...\n");
        player = msg.character;
    } else {
        printf("Server sent an invalid message\n");
        exit(1);
    }

    while (1) {
        // wait for "STATE_UPDATE" or "GAME_OVER" message
        if (receive_message(sockfd, &msg) == -1) {
            printf("Failed to receive message from the server\n");
            exit(1);
        }
        // check if the game is over
        if (strcmp(msg.type, MSG_GAME_OVER) == 0) {
            if (msg.character == player) {
                printf("You win!\n");
            } else if (msg.character == EMPTY) {
                printf("It's a tie!\n");
            } else {
                printf("You lose!\n");
            }
            break;
        }
        // check if the message is a state update
        if (strcmp(msg.type, MSG_STATE_UPDATE) == 0) {
            // update the board and print it
            memcpy(board, msg.board, sizeof(Board));
            print_board(board);
        } else {
            printf("Server sent an invalid message\n");
            exit(1);
        }

        while (1) {
            // player's turn
            printf("Enter the position you want to play (1-9): ");
            const char position = getchar();
            getchar(); // Consume the newline character
            if (atoi(&position) >= 1 && atoi(&position) <= 9) {
                if (board[atoi(&position) - 1] == EMPTY) {
                    // send the move to the server
                    board[atoi(&position) - 1] = player;
                    strcpy(msg.type, MSG_MOVE);
                    memcpy(msg.board, board, sizeof(Board));
                    send_message(sockfd, &msg);
                    break;
                }
                printf("Invalid position selected\n");
            }
        }
    }
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
