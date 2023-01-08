#include "../include/server.h"

void* ThreadCreatesMsg(void* arg) {

    key_t key = (key_t) (__intptr_t) arg;

    int msgId = msgget(key, IPC_CREAT | 0660);
    if (msgId == -1) {
        perror("[ERROR] Erreur lors de la création de la boite aux lettres");
        exit(EXIT_FAILURE);
    }
    else {
        if (DEBUG_MODE) {
            printf("[DEBUG] Création de la boite aux lettres n° %d\n", msgId);
        }
    }
    return (void*) (__intptr_t) msgId;
}

void InitParty(Party* party) {
    (*party).numberPlayers = 0;
}

Player WaitForAPlayer(int msgId) {
    WrapPlayer player;

    if (msgrcv(msgId, &player, sizeof(player.mtext), 1, 0) == -1) {
        perror("[ERROR][SERV] Erreur lors de la lecture du message du client");
        exit(EXIT_FAILURE);
    }

    return player.mtext;
}

void WaitingForPlayers(Party* party) {
    Player playerBuffer;

    while ((*party).numberPlayers < MAX_NUMBER_PLAYER) {
        printf("En attente de joueurs (%d/%d)...\n", (*party).numberPlayers, MAX_NUMBER_PLAYER);
        // Receiving a player from client
        playerBuffer = WaitForAPlayer(msgId);
        printf("Player %d joined the game\n", (*party).numberPlayers) + 1;
        printf("Client n°%d\n", playerBuffer.pid);
        printf("Player : %s\n", playerBuffer.name);

        // Store player received in the party
        InsertPlayerInParty(&(*party), playerBuffer);

        // Send the current party to all currents players
        SendCurrentPartyToCurrentsPlayers(*party);

    }
}

void SendCurrentPartyToCurrentsPlayers(Party party) {
    pthread_t senders[MAX_NUMBER_PLAYER];
    for (int threadNumber = 0; threadNumber < party.numberPlayers; threadNumber++) {
        SenderArgsParty args;
        // Lock args variable modification
        pthread_mutex_lock(&mutexParty);
        args = SendersArgsPartyConstructor(party.playersTab[threadNumber].pid, party);
        pthread_create(
            &senders[threadNumber],
            NULL,
            ThreadSendsParty,
            &args
        );
    }

    for (int threadNumber = 0; threadNumber < party.numberPlayers; threadNumber++) {
        pthread_join(senders[threadNumber], NULL);
    }
}

void InsertPlayerInParty(Party* party, Player player) {
    (*party).playersTab[(*party).numberPlayers] = player;
    (*party).numberPlayers++;
}

WrapParty WrapPartyConstructor(Party party, pid_t receiver) {
    WrapParty partyInAWrap;

    partyInAWrap.mtext = party;
    partyInAWrap.mtype = (long) receiver;

    return partyInAWrap;
}

void SendPartyInAWrap(WrapParty wrap) {
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

SenderArgsParty SendersArgsPartyConstructor(pid_t pid, Party party) {
    SenderArgsParty args;
    args.receiver = pid;
    args.party = party;
    return args;
}


void* ThreadSendsParty(void* args) {
    // Get given argument
    SenderArgsParty* data = (SenderArgsParty*) args;

    // Transform data in usable variable
    pid_t receiver = (*data).receiver;
    Party party = (*data).party;
    pthread_mutex_unlock(&mutexParty);
    //! Don't use args variable here

    SendPartyInAWrap(WrapPartyConstructor(party, receiver));
    // Faire quelque chose dans le thread...
    //printf("Je suis le thread numéro %d !\n", threadNumber);

    return NULL;
}

int InitServer() {
    int nbInitializer = 1;
    pthread_t initializer[nbInitializer];
    int i;

    int shmId;
    void* msgId;
    key_t key;

    key = CreateKey();

    pthread_create(
        &initializer[0],
        NULL,
        ThreadCreatesMsg,
        (void*) (__intptr_t) key
    );


    for (i = 0; i < nbInitializer; i++) {
        pthread_join(initializer[i], &msgId);
    }
    return (int) (__intptr_t) msgId;
}

void PlayAGame(Party party) {
    Game game;
    pthread_t senders[MAX_NUMBER_PLAYER];
    InitAGame(&game);
    do {
        SendCurrentGameToCurrentsPlayers(game, party);
    } while (game.isInProgress);

}

void SendCurrentGameToCurrentsPlayers(Game game, Party party) {
    pthread_t senders[MAX_NUMBER_PLAYER];

    for (int threadNumber = 0; threadNumber < party.numberPlayers; threadNumber++) {
        SenderArgsGame args;
        pthread_mutex_lock(&mutexGame);
        args = SendersArgsGameConstructor(party.playersTab[threadNumber].pid, game);
        pthread_create(
            &senders[threadNumber],
            NULL,
            ThreadSendsGame,
            &args
        );
    }

    for (int threadNumber = 0; threadNumber < party.numberPlayers; threadNumber++) {
        pthread_join(senders[threadNumber], NULL);
    }
}

void* ThreadSendsGame(void* args) {
    // Get given argument
    SenderArgsGame* data = (SenderArgsGame*) args;

    // Transform data in usable variable
    pid_t receiver = (*data).receiver;
    Game game = (*data).game;
    pthread_mutex_unlock(&mutexGame);
    //! Don't use args variable here

    SendGameInAWrap(WrapGameConstructor(game, receiver));
    // Faire quelque chose dans le thread...
    //printf("Je suis le thread numéro %d !\n", threadNumber);

    return NULL;
}

WrapGame WrapGameConstructor(Game game, pid_t receiver) {
    WrapGame gameInAWrap;

    gameInAWrap.mtext = game;
    gameInAWrap.mtype = (long) receiver;

    return gameInAWrap;
}

void SendGameInAWrap(WrapGame wrap) {

    if (msgsnd(msgId, &wrap, sizeof(wrap.mtext),0) == -1) {
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

SenderArgsGame SendersArgsGameConstructor(pid_t pid, Game game) {
    SenderArgsGame args;
    args.receiver = pid;
    args.game = game;
    return args;
}
