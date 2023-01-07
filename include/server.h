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
    int msgId;
    pid_t receiver;
    Party party;
}SenderArgs;

pthread_mutex_t mutex;


void SendPartyInAWrap(int msgId, WrapParty wrap);
void* ThreadCreatesMsg(void* arg);
void InitParty(Party* party);
Player WaitForAPlayer(int msgId);
void InsertPlayerInParty(Party* party, Player player);
void WaitingForPlayers(int msgId, Party* party);
void* ThreadSendsParty(void* args);
SenderArgs SendersArgsConstructor(int msgid, pid_t pid, Party party);
WrapParty Wrap(Party party, pid_t receiver);
void SendCurrentPartyToCurrentsPlayers(int msgId, Party party);
int InitServer();
void* ThreadCreatesShm(void* arg);