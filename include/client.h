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
void AskPlayerInfos(Player* player);
WrapPlayer Wrap(Player player);
void SendPlayerInAWrap(WrapPlayer wrap);
Group WaitForAGroup() ;
Group WaitForAFullGroup();
void* ThreadConnectsToShm(void* arg) ;
int InitClient();
void* ThreadReceivesGameUpdates();
void PlayAGame();
void* ThreadDisplaysAGame(void* arg) ;
void SendGame(Game game);
void* ThreadSendsInput(void* arg);