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

void WaitingForPlayers(int msgId, Party* party) {
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
        SendCurrentPartyToCurrentsPlayers(msgId, *party);

    }
}

void SendCurrentPartyToCurrentsPlayers(int msgId, Party party) {
    pthread_t senders[MAX_NUMBER_PLAYER];
    for (int threadNumber = 0; threadNumber < party.numberPlayers; threadNumber++) {
        SenderArgs args;
        // Lock args variable modification
        pthread_mutex_lock(&mutex);
        args = SendersArgsConstructor(msgId, party.playersTab[threadNumber].pid, party);
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

WrapParty Wrap(Party party, pid_t receiver) {
    WrapParty partyInAWrap;

    partyInAWrap.mtext = party;
    partyInAWrap.mtype = receiver;

    return partyInAWrap;
}

void SendPartyInAWrap(int msgId, WrapParty wrap) {
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

SenderArgs SendersArgsConstructor(int msgid, pid_t pid, Party party) {
    SenderArgs args;
    args.msgId = msgid;
    args.receiver = pid;
    args.party = party;
    return args;
}

void* ThreadSendsParty(void* args) {
    // Get given argument
    SenderArgs* data = (SenderArgs*) args;

    // Transform data in usable variable
    int msgId = (*data).msgId;
    pid_t receiver = (*data).receiver;
    Party party = (*data).party;
    pthread_mutex_unlock(&mutex);
    //! Don't use args variable here

    SendPartyInAWrap(msgId, Wrap(party, receiver));
    // Faire quelque chose dans le thread...
    //printf("Je suis le thread numéro %d !\n", threadNumber);

    return NULL;
}

int InitServer() {
    int nbInitializer = 2;
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

    pthread_create(
        &initializer[1],
        NULL,
        ThreadCreatesShm,
        (void*) (__intptr_t) key
    );

    for (i = 0; i < nbInitializer; i++) {
        pthread_join(initializer[i], &msgId);
    }
    return (int) (__intptr_t) msgId;

}

void* ThreadCreatesShm(void* arg) {
    int shmId;
    void* shm;

    key_t key = (key_t) (__intptr_t) arg;

    // Création du segment de mémoire partagée
    shmId = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);

    // Attachement du segment de mémoire partagée
    shm = shmat(shmId, NULL, 0);

    strcpy(shm, FORBIDDEN_CHAR);

    shmdt(shm);
}