/**
 * @file core.h
 * @author Timothe Boone & Inès Guitton
 * @brief
 * @version 1.0
 * @date 17/12/2022
 *
 *
 */

#include "game.h"
#include <stdlib.h>
#include <unistd.h>

#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <errno.h>


#define KEY_FILE "./key/key"
#define PROJECT_ID 42

#define FORBIDDEN_CHAR  "#@.*+"
#define MAX_NUMBER_PLAYER 1

#define MAX_PLAYER_NAME_LENGTH 64

#define SHM_SIZE (5 + MAX_NUMBER_PLAYER)

#if DEBUG
#define DEBUG_MODE 1
#else
#define DEBUG_MODE 0
#endif

typedef struct {
  char name[MAX_PLAYER_NAME_LENGTH];
  char character;
  pid_t pid;
} Player;

typedef struct {
  long mtype;
  Player mtext;
} WrapPlayer;

typedef struct {
  unsigned int numberPlayers;
  Player playersTab[MAX_NUMBER_PLAYER];
} Party;

typedef struct {
  long mtype;
  Party mtext;
} WrapParty;

typedef struct {
  long mtype;
  Game mtext;
} WrapGame;

key_t CreateKey();
void PrintMsgInfo(int msgId);
void CheckKeyFileExistence() ;