/**
 * @file main.c
 * @author Timothe Boone & Inès Guitton
 * @brief Client program
 * @version 1.0
 * @date 16/11/2022
 *
 */
#include "../include/client.h"

int main(int argc, char const* argv[]) {
  // Initialization of variables
  Player me;
  Party myParty;
  key_t key;
  int msgId;
  int shmId;
  int i;
  char forbiddenString[SHM_SIZE];
  // Initialize player structure
  InitPlayer(&me);

  InitClient(&msgId, &shmId);

  // Find the message queue created by the server
  //msgId = ThreadConnectsToMsg();
  printf("main shm : %d\n", shmId);
  // Retrieve the forbidden string from shared memory
  
  printf("Choose your character? (except %s) \n", GetForbiddenString(shmId));
  // Creation of players
  AskPlayerInfos(&me, shmId);

  // Player created
  // Send my player to the server
  SendPlayerInAWrap(msgId, Wrap(me));

  // Summary of the players registered in the game = lobby
  myParty = WaitForAFullParty(msgId);
  printf("Lancement du jeu\n");
  // Store player
}
/****************************************************** JEU ***************************************************************************/

// Boite aux lettres
// Chacun stock le sien

// Game
// Show map
// Edit map
// Edit map when input
// Players movement
// Boxes
// # : mur
// @ : caisse
// . : destination
// * : caisse sur une zone de rangement (pas présente dans ce niveau)
// + : personnage sur une zone de rangement (pas présent dans ce niveau)

// Destination
// Walls
// Dont go forward
// Victory
// Go back

// Lock detection
