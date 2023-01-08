/**
 * @file server.h
 * @author Timothe Boone & Inès Guitton
 * @brief
 * @version 1.0
 * @date 17/12/2022
 *
 *
 */
#include "core.h"



typedef struct {
    pid_t receiver;
    Group party;
}SenderArgsGroup;

typedef struct {
    pid_t receiver;
    Game game;
}SenderArgsGame;

pthread_mutex_t mutexGroup;
pthread_mutex_t mutexGame;


void SendGroupInAWrap(WrapGroup wrap);
void* ThreadCreatesMsg(void* arg);
void InitGroup(Group* party);
Player WaitForAPlayer();
void InsertPlayerInGroup(Group* party, Player player);
void WaitingForPlayers( Group* party);
void* ThreadSendsGroup(void* args);
SenderArgsGroup SendersArgsGroupConstructor( pid_t pid, Group party);
WrapGroup WrapGroupConstructor(Group party, pid_t receiver);
void SendCurrentGroupToCurrentsPlayers(Group party);
int InitServer();
void PlayAGame();
SenderArgsGame SendersArgsGameConstructor(pid_t pid, Game game);
void SendGameInAWrap(WrapGame wrap);
WrapGame WrapGameConstructor(Game game, pid_t receiver) ;
void* ThreadSendsGame(void* args);
void SendCurrentGameToCurrentsPlayers(Game game, Group party);

