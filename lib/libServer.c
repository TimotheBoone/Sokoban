#include "../include/server.h"

void SendPartyInAWrap(int msgId, WrapParty wrapParty, pid_t receiver) {
    if (msgsnd(msgId, &wrapParty, sizeof(wrapParty.mtext), 0) == -1) {
        perror("[ERROR] Erreur lors de l'écriture du message");
        exit(EXIT_FAILURE);
    }
    else {
        if (DEBUG_MODE) {
            printf("[DEBUG] Ecriture du message dans la boite aux lettres n° %d\n", msgId);
        }

    }
}

int CreateMsg(key_t key) {
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
    return msgId;
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

        printf("Client n°%d\n", playerBuffer.pid);
        printf("Player : %s\n", playerBuffer.name);

        // Store player received in the party
        InsertPlayerInParty(&(*party), playerBuffer);
    }
}


void InsertPlayerInParty(Party* party, Player player) {
    (*party).playersTab[(*party).numberPlayers] = player;
    (*party).numberPlayers++;
}