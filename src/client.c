/**
 * @file client.c
 * @author Timothe Boone & Inès Guitton
 * @brief Client program
 * @version 1.0
 * @date 16/11/2022
 *
 */
#include "../include/client.h"

int main(int argc, char const* argv[]) {
  //* Initialization of variables

  // Data transfer structures
  Player me;
  // Key known by server and clients
  key_t key;
  // Ids of protocols that will transfer data server<->clients
  int msgId;
  int shmId;

  //* Connection client<->server
  // Initialize data transfer protocols
  //  - Msg, for Party structure
  //  - Shm, for forbidden character string
  InitClient(&msgId, &shmId);

  // Player structure filled by the user
  AskPlayerInfos(&me, shmId);
  // Send my player to the server
  SendPlayerInAWrap(msgId, Wrap(me));

  // Summary of the players registered in the game
  WaitForAFullParty(msgId);

  //* Game is ready, launch the game
  PlayAGame(msgId);

}