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
  int msgId;                // Id of msg
  Party lobby;

       // Array of senders threads

  pthread_mutex_init(&mutex, NULL);

  // Create a message queue
  msgId = InitServer();

  // Print msg infos
  if (DEBUG_MODE) {
    PrintMsgInfo(msgId);
  }

  InitParty(&lobby);

  // Waiting for players connection
  WaitingForPlayers(msgId, &lobby);
  

}

