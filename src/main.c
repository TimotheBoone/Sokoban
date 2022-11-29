/**
 * @file main.c
 * @author Timothe Boone & Inès Guitton
 * @brief Main program
 * @version 1.0
 * @date 16/11/2022
 *
 */
#include "../include/main.h"

void main() {

    // Initialization of variables
    Player me;
    strcpy(me.name, "");
    char playerInputBuffer[MAX_PLAYER_NAME_LENGTH];

    // Players conections

    // Hote - Serveur - Premier créé


    // Suivants , Se connecte à l'hote


// Pseudo requests
    do {
        printf("What's your player name ?\n");

        // Get user input
        fgets(playerInputBuffer, MAX_PLAYER_NAME_LENGTH, stdin);

        // Check if the string is ended or too long
        if (playerInputBuffer[strlen(playerInputBuffer) - 1] != '\n') {
            // String is too long
            ErrorInputStringTooLong();
        }
        else {
            // String length is correct

            // Remove ending newline
            playerInputBuffer[strlen(playerInputBuffer) - 1] = '\0';

            // Set the name of the player at the buffer value
            strcpy(me.name, playerInputBuffer);
            printf("My name is %s\n", me.name);
            
        }

        //TODO : Boucle si erreur
    } while (strcmp(me.name, "")==0);


    // Mémoire partagée
    // Chacun stock le sien

// Choose your character
    // @ & § 
    // Memoire partagée

    //Ines&
    //Tim§

// Game
    // Show map
    // Edit map
    // Edit map when input
    // Players movement
    // Go back
    // Boxes
    // Destination
    // Walls
        // Dont go forward
    // Victory


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