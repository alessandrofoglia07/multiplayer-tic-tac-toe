#include "server.h"
#include <pthread.h>

Game games[MAX_GAMES];
pthread_mutex_t games_mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    return 0;
}
