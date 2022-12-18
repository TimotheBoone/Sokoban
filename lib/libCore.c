/**
 * @file libCore.c
 * @author Timothe Boone & Inès Guitton
 * @brief
 * @version 1.0
 * @date 17/12/2022
 *
 *
 */

#include "../include/core.h"

 /**
  * @brief Generate a key
  *
  * @return key_t
  */
key_t CreateKey() {
    key_t key;

    // Check key file existence
    CheckKeyFileExistence();


    key = ftok(KEY_FILE, PROJECT_ID);
    if (key == -1) {
        perror("[ERROR] Erreur lors de la création de la clé");
        exit(EXIT_FAILURE);
    }
    else {
        if (DEBUG_MODE) {
            printf("[DEBUG] Clé créée\n");
        }
    }

    return key;
}

void CheckKeyFileExistence() {
    FILE* file = fopen(KEY_FILE, "w");

    if (file == NULL) {
        // Error creating file
        perror("[ERROR] Erreur lors de la création du fichier clé");
        exit(EXIT_FAILURE);
    }
    else {
        if (DEBUG_MODE) {
            printf("[DEBUG] Le fichier clé est accessible\n");
        }
    }
    fclose(file);
}

// Affiche les informations de la boite aux lettres
void PrintMsgInfo(int msgId) {
    // Déclaration des variables
    struct msqid_ds buf;

    if (msgctl(msgId, IPC_STAT, &buf) == -1) {
        perror(
            "[ERROR] Erreur lors de la lecture des informations de la boite aux "
            "lettres");
        exit(EXIT_FAILURE);
    }
    else {
        printf("[SERV] Lecture des informations de la boite aux lettres n° %d\n",
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


