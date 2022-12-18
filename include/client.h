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
int ConnectToMsg(key_t key);
void AskPlayerInfos(Player* player);
WrapPlayer Wrap(Player player);
void SendPlayerInAWrap(int msgId, WrapPlayer wrap);
