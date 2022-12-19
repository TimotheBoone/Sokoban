/**
 * @file libClient.c
 * @author Timothe Boone & Inès Guitton
 * @brief
 * @version 1.0
 * @date 17/12/2022
 *
 *
 */
# include "../include/client.h"

int ConnectToMsg(key_t key) {
    int msgId;

    msgId = msgget(key, IPC_EXCL);
    if (msgId == -1) {
        perror("[ERROR] Erreur lors de la recherche de la boite aux lettres");
        exit(EXIT_FAILURE);
    }
    else {
        if (DEBUG_MODE) {
            printf("[DEBUG] Connexion à la boite aux lettres n° %d\n", msgId);
        }
    }

    return msgId;
}

void InitPlayer(Player* player) {
    strcpy((*player).name, "");
    (*player).pid = getpid();
}

void ErrorInputStringTooLong() {
    // Notifying the user
    fprintf(stderr, "[ERROR]: The input was too long, please try again.\n");
    // Clear the input buffer
    fflush(stdin);
}

void AskPlayerInfos(Player* player) {
    char playerNameBuffer[MAX_PLAYER_NAME_LENGTH];
    char playerCharacterBuffer[1];
    do {
        printf("What's your player name ?\n");

        // Get user input
        fgets(playerNameBuffer, MAX_PLAYER_NAME_LENGTH, stdin);

        // Check if the string is ended or too long
        if (playerNameBuffer[strlen(playerNameBuffer) - 1] != '\n') {
            // String is too long
            ErrorInputStringTooLong();
            // Reset Buffer
            strcpy(playerNameBuffer, "");
        }
        else {
            // String length is correct

            // Remove ending newline
            playerNameBuffer[strlen(playerNameBuffer) - 1] = '\0';

            // Set the name of the player at the buffer value
            strcpy((*player).name, playerNameBuffer);
            printf("My name is %s\n", (*player).name);
        }

    } while (strlen((*player).name) <= 0);

    // do {
    printf("Choose your character? (except #, @,. ,*, +) \n");
    fgets(playerCharacterBuffer, 2, stdin);

    // We research char in this string
    // If it's not in it, we put it into the struct
    if (strchr(FORBIDDEN_CHAR, playerCharacterBuffer[0]) == NULL) {
        (*player).character = playerCharacterBuffer[0];
    }
    else {
        printf(
            "[ERROR] You're not allowed to use this character ! Please choose "
            "another one.\n");
    }

    //} while (sizeof(me.name) >= 0);
}

WrapPlayer Wrap(Player player) {
    WrapPlayer playerInAWrap;

    playerInAWrap.mtext = player;
    playerInAWrap.mtype = 1;

    return playerInAWrap;
}

void SendPlayerInAWrap(int msgId, WrapPlayer wrap) {
    if (msgsnd(msgId, &wrap, sizeof(wrap.mtext), 0) == -1) {
        perror("[ERROR] Erreur lors de l'écriture du message");
        exit(EXIT_FAILURE);
    }
    else {
        if (DEBUG_MODE) {
            printf("[DEBUG] Ecriture du joueur dans la boite aux lettres n° %d\n",
                   msgId);
        }
    }
}

//Party WaitForAParty