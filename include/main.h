#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_NUMBER_PLAYER 3
#define MAX_PLAYER_NAME_LENGTH 64


static inline void ErrorInputStringTooLong();
static inline void ClearInputBuffer();

typedef struct {
    char name[MAX_PLAYER_NAME_LENGTH];
    char character;
} Player;


typedef struct {
    unsigned int numberPlayers;
    Player playersTab[MAX_NUMBER_PLAYER];
} Party;


