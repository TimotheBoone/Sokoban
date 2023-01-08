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

int InitClient(int* ptrMsgId) {
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

    pthread_join(initializer[0], &msgId);

    *ptrMsgId = (int) (__intptr_t) msgId;
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

void AskPlayerInfos(Player* player) {
    char playerNameBuffer[MAX_PLAYER_NAME_LENGTH];
    char playerCharacterBuffer[1];
    //char *forbiddenString;

    InitPlayer(player);

    do {
        printf("What's your player name ?\n");

        // Get user input
        fgets(playerNameBuffer, MAX_PLAYER_NAME_LENGTH, stdin);

        // Check if the string is ended or too l
            // String length is correct

            // Remove ending newline
        playerNameBuffer[strlen(playerNameBuffer) - 1] = '\0';

        // Set the name of the player at the buffer value
        strcpy((*player).name, playerNameBuffer);
        printf("My name is %s\n", (*player).name);
    }

    while (strlen((*player).name) <= 0);

    fgets(playerCharacterBuffer, 2, stdin);

    // We research char in this string
    // If it's not in it, we put it into the struct


    //} while (strlen((*player).character) <= 0);
}

void InitPlayer(Player* player) {
    strcpy((*player).name, "");
    (*player).pid = getpid();
}

WrapPlayer Wrap(Player player) {
    WrapPlayer playerInAWrap;

    playerInAWrap.mtext = player;
    playerInAWrap.mtype = 1;

    return playerInAWrap;
}

void SendPlayerInAWrap(WrapPlayer wrap) {
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

Group WaitForAGroup() {
    WrapGroup party;
    if (msgrcv(msgId, &party, sizeof(party.mtext), getpid(), 0) == -1) {
        perror("[ERROR] Erreur lors de la lecture du message du serveur");
        exit(EXIT_FAILURE);
    }

    return party.mtext;
}

Game* WaitForAGame() {
    WrapGame wrapGame;
    Game* game;
    game = (Game*) malloc(sizeof(Game*));

    if (game == NULL) {
        perror("Erreur malloc");
        exit(EXIT_FAILURE);
    }

    if (msgrcv(msgId, &wrapGame, sizeof(wrapGame.mtext), getpid(), 0) == -1) {
        perror("[ERROR] Erreur lors de la lecture du message du serveur");
        exit(EXIT_FAILURE);
    }

    *game = wrapGame.mtext;

    return game;
}

Group WaitForAFullGroup() {
    int i;
    Group party;
    do {
        party = WaitForAGroup(msgId);
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

void PlayAGame() {
    Game* game;
    pthread_t displayThread, inputThread;


    pthread_mutex_init(&gameLocker, NULL);
    pthread_cond_init(&gameUpdate, NULL);

    printf("Waiting for the game launching...\n");
    game = WaitForAGame();

    pthread_create(&displayThread, NULL, ThreadDisplaysAGame, (void*) &game);
    pthread_create(&inputThread, NULL, ThreadSendsInput, (void*) &game);

    do {
        pthread_mutex_lock(&gameLocker);

        game = WaitForAGame();

        pthread_cond_signal(&gameUpdate);
    } while (game->isInProgress);

    pthread_join(displayThread, NULL);

    free(game);
}

void* ThreadDisplaysAGame(void* arg) {
    WINDOW* win = NULL;
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
        pthread_cond_wait(&gameUpdate, &gameLocker);

        localGame = *game;
        pthread_mutex_unlock(&gameLocker);
        //! Don't use game here
        Display(&localGame, win);

    }

    return NULL;
}

void* ThreadSendsInput(void* arg) {
    Game* game = (Game*) arg;
    while (game->isInProgress) {
        UserInput(game, 0);

        SendGame(*game);
    }
}


void SendGame(Game game) {

    WrapGame wrap;
    wrap.mtext = game;
    wrap.mtype = 1;

    if (msgsnd(msgId, &wrap, sizeof(wrap.mtext), 0) == -1) {
        perror("[ERROR] Erreur lors de l'écriture du message");
        exit(EXIT_FAILURE);
    }
    else {
        if (DEBUG_MODE) {
            printf("[DEBUG] Ecriture de la partie dans la boite aux lettres n° %d\n",
                   msgId);
        }
    }
}