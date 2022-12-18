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

void SendParty(int msgId, WrapParty wrapParty, pid_t receiver);
int CreateMsg(key_t key);
void InitParty(Party* party);
Player WaitForAPlayer(int msgId);
void InsertPlayerInParty(Party* party, Player player);
void WaitingForPlayers(int msgId, Party* party);