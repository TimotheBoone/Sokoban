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

    *ptrMsgId = (int) (__intptr_t) msgId;
    *ptrShmId = (int) (__intptr_t) shmId;
}

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

    return (void*) (__intptr_t) shmId;
}



void AskPlayerInfos(Player* player, int shmId) {
    char playerNameBuffer[MAX_PLAYER_NAME_LENGTH];
    char playerCharacterBuffer[1];
    //char *forbiddenString;

    InitPlayer(player);

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

Game WaitForAGame(int msgId) {
    WrapGame game;

    if (msgrcv(msgId, &game, sizeof(game.mtext), getpid(), 0) == -1) {
        perror("[ERROR] Erreur lors de la lecture du message du serveur");
        exit(EXIT_FAILURE);
    }

    return game.mtext;
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


void PlayAGame(int msgId) {
    Game* game;
    pthread_t displayThread;
    pthread_create(&displayThread, NULL, ThreadDisplaysAGame, (void*) &game);

    InitAGame(game);
    printf("C'est OK ici !\n");
    do {
        pthread_mutex_lock(&(game->gameLocker));
        // Update the game state
        *game = WaitForAGame(msgId);

        pthread_cond_signal(&(game->gameUpdate));
    } while (1);

    pthread_join(displayThread, NULL);
}

void* ThreadDisplaysAGame(void* arg) {
    WINDOW* win = NULL ;
    Game* game = (Game*) arg;
    Game localGame;
    int i;//Compteur
    initscr();
    curs_set(false);
    refresh();
    newwin(GAME_HEIGHT + INFO_HEIGHT + 2, GAME_WIDTH + 2, (LINES - GAME_HEIGHT + INFO_HEIGHT + 2) / 2, (COLS - GAME_WIDTH + 2) / 2);
    box(win, 0, 0);
    wrefresh(win);
    keypad(win, true);
    nodelay(win, true);
    while (game->isInProgress) {
        pthread_cond_wait(&(game->gameUpdate), &(game->gameLocker));

        localGame = *game;
        pthread_mutex_unlock(&(game->gameLocker));
        //Avant chaque affichage, on refresh pour mettre à jour le plateau et prendre en compte les mouvements définis par la fonction move_charac()
        werase(win);
        box(win, 0, 0);

        //On fait appel à la fonction qui s'occupe de l'affichage des murs du plateau 

        //Affichage d'une caisse
        for (i = 0; i < localGame.boxs.numberElt;i++) {
            mvwprintw(win, (localGame.boxs.arrayElt[i].pos.y) + 1, (localGame.boxs.arrayElt[i].pos.x) + 1, "+");
        }

        //Affichage d'un receptacle
        for (i = 0; i < localGame.containers.numberElt;i++) {
            mvwprintw(win, (localGame.containers.arrayElt[i].y) + 1, (localGame.containers.arrayElt[i].x) + 1, ".");
        }
        //Affichage d'un mur 
        for (i = 0; i < localGame.walls.numberElt;i++) {
            mvwprintw(win, (localGame.walls.arrayElt[i].y) + 1, (localGame.walls.arrayElt[i].x) + 1, "#");
        }
        //Affichage d'un personnage
        for (i = 0; i < localGame.characters.numberElt;i++) {
            mvwprintw(win, (localGame.characters.arrayElt[i].pos.y) + 1, (localGame.characters.arrayElt[i].pos.x) + 1, "@");
        }

        //Toujours refresh l'écran pour permettre l'affichage lorsqu'on utlise la biliothèque ncurses 
        wrefresh(win);
    }

    return NULL;
}