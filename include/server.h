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
    Party party;
}SenderArgsParty;

typedef struct {
    pid_t receiver;
    Game game;
}SenderArgsGame;

pthread_mutex_t mutexParty;
pthread_mutex_t mutexGame;


void SendPartyInAWrap(WrapParty wrap);
void* ThreadCreatesMsg(void* arg);
void InitParty(Party* party);
Player WaitForAPlayer();
void InsertPlayerInParty(Party* party, Player player);
void WaitingForPlayers( Party* party);
void* ThreadSendsParty(void* args);
SenderArgsParty SendersArgsPartyConstructor( pid_t pid, Party party);
WrapParty WrapPartyConstructor(Party party, pid_t receiver);
void SendCurrentPartyToCurrentsPlayers(Party party);
int InitServer();
void PlayAGame();
SenderArgsGame SendersArgsGameConstructor(pid_t pid, Game game);
void SendGameInAWrap(WrapGame wrap);
WrapGame WrapGameConstructor(Game game, pid_t receiver) ;
void* ThreadSendsGame(void* args);
void SendCurrentGameToCurrentsPlayers(Game game, Party party);

