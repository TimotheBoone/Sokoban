/**
 * @file client.h
 * @author Timothe Boone & Inès Guitton
 * @brief
 * @version 1.0
 * @date 17/12/2022
 *
 *
 */
#include "core.h"

void ErrorInputStringTooLong();
void InitPlayer(Player* player);
void* ThreadConnectsToMsg(void* arg);
void AskPlayerInfos(Player* player, int shmId);
WrapPlayer Wrap(Player player);
void SendPlayerInAWrap(int msgId, WrapPlayer wrap);
Party WaitForAParty(int msgId) ;
Party WaitForAFullParty(int msgId);
void* ThreadConnectsToShm(void* arg) ;
int InitClient(int* ptrMsgId, int* ptrShmId);
char* GetForbiddenString(int shmId);
void* ThreadReceivesGameUpdates(void* ptrMsgId);
void PlayAGame(int msgId);
void* ThreadDisplaysAGame(void* arg) ;