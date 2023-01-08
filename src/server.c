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
  //* Initialization of variables

  // Datastructure used for connecting players
  Party lobby;
  // Key known by server and clients
  key_t key;
  // Id of msg that will transfer data server<->clients
  int msgId;
  int shmId;

  //* Initialization of mutex
  pthread_mutex_init(&mutex, NULL);

  //* Connection server<->clients
  // Initialize data transfer protocols
  //  - Msg, for Party structure
  //  - Shm, for forbidden character string
  msgId = InitServer();

  // Check infos about created msg
  if (DEBUG_MODE) {
    PrintMsgInfo(msgId);
  }

  // Initialize default datas for party
  InitParty(&lobby);

  // Waiting for all players connection
  WaitingForPlayers(msgId, &lobby);

  //* Launch the game
  printf("Game launched\n");

}

