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

#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define MAX_NUMBER_PLAYER 3
#define MAX_PLAYER_NAME_LENGTH 64
#define KEY_FILE "./key/key"


static inline void ErrorInputStringTooLong();
static inline void ClearInputBuffer();

typedef struct {
    char name[MAX_PLAYER_NAME_LENGTH];
    char character;
    pid_t pid;
} Player;


typedef struct {
    unsigned int numberPlayers;
    Player playersTab[MAX_NUMBER_PLAYER];
} Party;

typedef struct {
  long mtype; 
  Player mtext;
} WrapPlayer;
