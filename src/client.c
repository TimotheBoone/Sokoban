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
  WrapParty myParty;
  key_t key;
  char playerNameBuffer[MAX_PLAYER_NAME_LENGTH];
  char characterBuffer[1];
  int msgId;
  pid_t myPid = getpid();
  int i;

  // Initialize player structure
  InitPlayer(&me);

  // Connection to the server
  key = CreateKey();

  // Find the message queue created by the server
  msgId = ConnectToMsg(key);

  // Creation of players
  AskPlayerInfos(&me);

  // Player created
  // Send my player to the server
  SendPlayerInAWrap(msgId, Wrap(me));

  // Summary of the players registered in the game = lobby

  if (msgrcv(msgId, &myParty, sizeof(myParty), myPid, 0) == -1) {
    perror("[ERROR][SERV] Erreur lors de la lecture du message du client");
    exit(EXIT_FAILURE);
  }
  else {
    printf("Joueurs dans la partie :\n");
    for (i = 0; i < MAX_NUMBER_PLAYER; i++) {
      printf("Joueur %d : %s\n", i, myParty.mtext.playersTab[i].name);
    }

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
}
