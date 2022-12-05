/**
 * @file main.c
 * @author Timothe Boone & Inès Guitton
 * @brief Main program
 * @version 1.0
 * @date 16/11/2022
 *
 */
#include "../include/main.h"

int main(int argc, char const* argv[]) {
  // Initialization of variables
  Player me;
  WrapPlayer myPlayer;
  WrapParty myParty;
  key_t key;
  char playerNameBuffer[MAX_PLAYER_NAME_LENGTH];
  char characterBuffer[1];
  int msgId;
  pid_t myPid = getpid();
  int i;

  // Connexion to the server
  key = ftok(KEY_FILE, 42);
  if (key == -1) {
    perror("[ERROR] Erreur lors de la création de la clé");
    exit(EXIT_FAILURE);
  } else {
    printf("[CLI] Clé créée\n");
  }

  // Find the message queue created by the server
  msgId = msgget(key, IPC_EXCL);
  if (msgId == -1) {
    perror("[ERROR] Erreur lors de la recherche de la boite aux lettres");
    exit(EXIT_FAILURE);
  } else {
    printf("[Bal3][CLI] Boite aux lettres n° %d trouvée\n", msgId);
  }

  // Creation of players
  // Pseudo requests
  // do {
  printf("What's your player name ?\n");

  // Get user input
  fgets(playerNameBuffer, MAX_PLAYER_NAME_LENGTH, stdin);

  // Check if the string is ended or too long
  if (playerNameBuffer[strlen(playerNameBuffer) - 1] != '\n') {
    // String is too long
    ErrorInputStringTooLong();
  } else {
    // String length is correct

    // Remove ending newline
    playerNameBuffer[strlen(playerNameBuffer) - 1] = '\0';

    // Set the name of the player at the buffer value
    strcpy(me.name, playerNameBuffer);
    printf("My name is %s\n", me.name);
  }

  // TODO : Boucle si erreur
  //} while (sizeof(me.name) == 1);

  // do {
  printf("Choose your character? (except #, @,. ,*, +) \n");
  fgets(characterBuffer, 2, stdin);

  // We research char in this string
  // If it's not in it, we put it into the struct
  if (strchr(FORBIDDEN_CHAR, characterBuffer[0]) == NULL) {
    me.character = characterBuffer[0];
  } else {
    printf(
        "[ERROR] You're not allowed to use this character ! Please choose "
        "another one.\n");
  }

  //} while (sizeof(me.name) >= 0);
  me.pid=myPid;
  // Player created
  // Send my player to the server
  myPlayer.mtext = me;
  myPlayer.mtype = 1;

  if (msgsnd(msgId, &myPlayer, sizeof(myPlayer), 0) == -1) {
    perror("[ERROR] Erreur lors de l'écriture du message");
    exit(EXIT_FAILURE);
  } else {
    printf("[Bal3][CLI] Ecriture du message dans la boite aux lettres n° %d\n",
           msgId);
  }

  // Summary of the players registered in the game = lobby

  if (msgrcv(msgId, &myParty, sizeof(myParty), myPid, 0) == -1) {
    perror("[ERROR][SERV] Erreur lors de la lecture du message du client");
    exit(EXIT_FAILURE);
  } else {
    printf("Joueurs dans la partie :\n");
    for (i = 0; i < MAX_NUMBER_PLAYER; i++) {
      printf("Joueur %d : %s\n", i, myParty.mtext.playersTab[i].name);
    }

    // Store player
  }

  // Boite aux lettres
  // Chacun stock le sien

  // Ines&
  // Tim§

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
void ErrorInputStringTooLong() {
  // Notifying the user
  fprintf(stderr, "[ERROR]: The input was too long, please try again.\n");
  // Clear the input buffer
  ClearInputBuffer();
}

void ClearInputBuffer() {
  fflush(stdin);
}