/**
 * @file server.c
 * @author Timothe Boone & Inès Guitton
 * @brief Server of sokoban
 * @version 1.0
 * @date 16/11/2022
 *
 */
#include "../include/main.h"
void printMsgInfo(int msgId);

int main(int argc, char const* argv[]) {
    // Initialization of variables
    key_t key;  // Used key
    int msgId;  // Id of msg
    Party lobby; // Lobby of the game
    Player playerBuffer; // Player container

    // Generating key
    key = ftok(KEY_FILE, 42);

    // Create msg
    msgId = msgget(key, IPC_CREAT | 0660);
    if (msgId == -1) {
        perror("[ERROR] Erreur lors de la création de la boite aux lettres");
        exit(EXIT_FAILURE);
    }
    else {
        printf("[SERV] Création de la boite aux lettres n° %d\n", msgId);
    }

    // Print msg infos
    printMsgInfo(msgId);

    // Server received a player
    // Add the player to the party
    // Send party to clients

    while (1) {
        if (msgrcv(msgId, &playerBuffer, sizeof(playerBuffer), 1, 0) == -1) {
            perror(
                "[ERROR][SERV] Erreur lors de la lecture du message du client");
            exit(EXIT_FAILURE);
        }
        else {
            printMsgInfo(msgId);
            printf("[SERV] Message recu par\n");
            printMsgInfo(msgId);
        }
    }
}

// Affiche les informations de la boite aux lettres
void printMsgInfo(int msgId) {
    // Déclaration des variables
    struct msqid_ds buf;

    if (msgctl(msgId, IPC_STAT, &buf) == -1) {
        perror(
            "[ERROR] Erreur lors de la lecture des informations de la boite aux "
            "lettres");
        exit(EXIT_FAILURE);
    }
    else {
        printf("SERV] Lecture des informations de la boite aux lettres n° %d\n",
               msgId);
    }

    printf("--- Informations sur la boite aux lettre n° %d ---\n", msgId);
    printf("Clé fournie à msgget : \t%d\n", buf.msg_perm.__key);
    printf("UID effectif du propriétaire : \t%d\n", buf.msg_perm.uid);
    printf("GID effectif du propriétaire : \t%d\n", buf.msg_perm.gid);
    printf("UID effectif du créateur : \t%d\n", buf.msg_perm.cuid);
    printf("GID effectif du créateur : \t%d\n", buf.msg_perm.cgid);
    printf("Permissions : \t%d\n", buf.msg_perm.mode);
    printf("Numéro de séquence : \t%d\n", buf.msg_perm.mode);
    printf("--\n");
    printf("Heure du dernier msgsnd : \t%ld\n", buf.msg_stime);
    printf("Heure du dernier msgrcv : \t%ld\n", buf.msg_rtime);
    printf("Heure de la dernière modif : \t%ld\n", buf.msg_ctime);
    printf("Nombre actuel d'octets dans la file (non standard) : \t%ld\n",
           buf.__msg_cbytes);
    printf("Nombre actuel de messages dans la file : \t%ld\n", buf.msg_qnum);
    printf("Nombre actuel de messages dans la file : \t%ld\n", buf.msg_qnum);
    printf("Nombre maximum d'octets autorisés dans la file : \t%ld\n",
           buf.msg_qbytes);
    printf("PID du dernier msgsnd : \t%d\n", buf.msg_lspid);
    printf("PID du dernier msgrcv : \t%d\n", buf.msg_lrpid);
    printf("\n");
}