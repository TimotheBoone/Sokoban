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

void* ThreadConnectsToMsg(void* arg) {

    key_t key = (key_t) (__intptr_t) arg;

    int msgId = msgget(key, IPC_EXCL);
    if (msgId == -1) {
        perror("[ERROR] Erreur lors de la recherche de la boite aux lettres");
        exit(EXIT_FAILURE);
    }
    else {
        if (DEBUG_MODE) {
            printf("[DEBUG] Connexion à la boite aux lettres n° %d\n", msgId);
        }
    }

    return (void*) (__intptr_t) msgId;
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

void AskPlayerInfos(Player* player, int shmId) {
    char playerNameBuffer[MAX_PLAYER_NAME_LENGTH];
    char playerCharacterBuffer[1];
    //char *forbiddenString;

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
    printf("main shm : %d\n", shmId);
    //do {
    //strcpy(forbiddenString, GetForbiddenString(shmId));
    //printf("Choose your character? (except %s) \n", GetForbiddenString(shmId));
    fgets(playerCharacterBuffer, 2, stdin);

    // We research char in this string
    // If it's not in it, we put it into the struct
    if (strchr(FORBIDDEN_CHAR, playerCharacterBuffer[0]) == NULL) {
        (*player).character = playerCharacterBuffer[0];
    }
    else {
        printf(
            "[ERROR] You're not allowed to use this character ! Please choose another one.\n");
    }

    //} while (strlen((*player).character) <= 0);
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

Party WaitForAParty(int msgId) {
    WrapParty party;

    if (msgrcv(msgId, &party, sizeof(party.mtext), getpid(), 0) == -1) {
        perror("[ERROR] Erreur lors de la lecture du message du serveur");
        exit(EXIT_FAILURE);
    }

    return party.mtext;
}

Party WaitForAFullParty(int msgId) {
    int i;
    Party party;
    do {
        party = WaitForAParty(msgId);
        if (party.numberPlayers < MAX_NUMBER_PLAYER) {
            printf("Waiting for other players (%d/%d)...\n", party.numberPlayers, MAX_NUMBER_PLAYER);
        }
    } while (party.numberPlayers < MAX_NUMBER_PLAYER);
    printf("Joueurs dans la partie :\n");
    for (i = 0; i < MAX_NUMBER_PLAYER; i++) {
        printf("Joueur %d : %s\n", i + 1, party.playersTab[i].name);
    }
    return party;
}

int InitClient(int* ptrMsgId, int* ptrShmId) {
    int nbInitializer = 1;
    pthread_t initializer[nbInitializer];

    void* shmId;
    void* msgId;
    key_t key;

    key = CreateKey();

    pthread_create(
        &initializer[0],
        NULL,
        ThreadConnectsToMsg,
        (void*) (__intptr_t) key
    );

    pthread_create(
        &initializer[1],
        NULL,
        ThreadConnectsToShm,
        (void*) (__intptr_t) key
    );

    pthread_join(initializer[0], &msgId);
    pthread_join(initializer[1], &shmId);

    ptrMsgId = msgId;
    ptrShmId = shmId;
}

void* ThreadConnectsToShm(void* arg) {

    key_t key = (key_t) (__intptr_t) arg;

    int shmId = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);

    if (shmId < 0) {
        // Erreur
        perror("[ERROR] Erreur lors de la recherche de mémoire partagée");
        exit(EXIT_FAILURE);
    }
    else {
        if (DEBUG_MODE) {
            printf("[DEBUG] Connexion à la mémoire partagée n° %d\n", shmId);
        }
    }
     printf("\n plaf:%s\n", GetForbiddenString(shmId));
      printf("\n plouf%s\n", GetForbiddenString(shmId));
    return (void*) (__intptr_t) shmId;
}

char* GetForbiddenString(int shmId) {
    char* string = malloc(SHM_SIZE);
    char* shm = shmat(shmId, NULL, 0);
    if (!shm) {
        perror("error");
        exit(EXIT_FAILURE);
    }
    else {
        if (DEBUG_MODE) {
            printf("[DEBUG] Attribution de la mémoire partagée");
        }
    }

    if(!strncpy(string, shm, SHM_SIZE)){
        perror("erroe");
        exit(EXIT_FAILURE);
    }

    shmdt(shm);

    return string;
}