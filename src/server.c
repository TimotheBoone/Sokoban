/**
 * @file server.c
 * @author Timothe Boone & Inès Guitton
 * @brief Server of sokoban
 * @version 1.0
 * @date 16/11/2022
 *
 */
#include "../include/server.h"


int main(int argc, char const* argv[]) {
  // Initialization of variables
  key_t key;                // Used key
  int msgId;  // Id of msg
  Party lobby;              // Lobby of the game
  Player playerBuffer;  // Player container
  int i;
  pthread_t th[3];
  WrapParty wrapLobby;

  // Generating key
  key = CreateKey();

  // Create a message queue
  msgId = CreateMsg(key);

  // Print msg infos
  if (DEBUG_MODE) {
    PrintMsgInfo(msgId);
  }

  InitParty(&lobby);

  // Waiting for players connection
  WaitingForPlayers(msgId, &lobby);

  

  // for (i = 0; i < MAX_NUMBER_PLAYER; i++) {
  //   // Send party to clients
  //   pthread_create(&th[i], NULL, SendParty, (void*) i);
  // }
}

