#include "game.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief Initialize game borders position
 *
 * @param game Pointer on the game structure
 * @param width Width of the game window
 * @param height Height of the game window
 */
void InitGameBorders(Game* game, int width, int height) {
    (*game).gameWidth = width;
    (*game).gameHeight = height;
}

/**
 * @brief Initialize an element position
 *
 * @param x X position of the element
 * @param y Y position of the element
 * @return Pos Element position structure
 */
Pos InitAnElement(int x, int y) {
    Pos element;
    element.x = x;
    element.y = y;
    return element;
}

/**
 * @brief Initialize a character
 *
 * @param x X position of the element
 * @param y Y position of the element
 * @return Player Character structure
 */
Movable InitAMovable(int x, int y) {
    Movable object;
    object.pos = InitAnElement(x, y);
    object.direction = -1;
    return object;
}

/**
 * @brief Initialize all game Elements as empty array
 *
 * @param game Pointer on the game structure
 */
void InitEmptyGameElements(Game* game) {
    Elements elements;
    elements.numberElt = 0;

    Movables movables;
    movables.numberElt = 0;

    (*game).characters = movables;
    (*game).boxs = movables;
    (*game).walls = elements;
    (*game).containers = elements;

    memset((*game).walls.arrayElt, 0, MAX_CASE * (sizeof(*game).walls.arrayElt[0]));
    memset((*game).boxs.arrayElt, 0, MAX_CASE * (sizeof(*game).boxs.arrayElt[0]));
    memset((*game).characters.arrayElt, 0, MAX_CASE * (sizeof(*game).characters.arrayElt[0]));
    memset((*game).containers.arrayElt, 0, MAX_CASE * (sizeof(*game).containers.arrayElt[0]));
}

/**
 * @brief Initialize a character position
 *
 * @param game Pointer on the game structure
 * @param x X position of the character
 * @param y Y position of the character
 */
void InitACharacter(Game* game, int x, int y) {
    // Insert character position in the Game structure
    if (game->characters.numberElt < MAX_CASE) {
        (*game).characters.arrayElt[(*game).characters.numberElt] = InitAMovable(x, y);
        (*game).characters.numberElt++;
    }
}

/**
 * @brief Initialize a wall position
 *
 * @param game Pointer on the game structure
 * @param x X position of the wall
 * @param y Y position of the wall
 */
void InitAWall(Game* game, int x, int y) {
    // Insert wall position in the Game structure
    (*game).walls.arrayElt[(*game).walls.numberElt] = InitAnElement(x, y);
    (*game).walls.numberElt++;
}

/**
 * @brief Initialize a box position
 *
 * @param game Pointer on the game structure
 * @param x X position of the wall
 * @param y Y position of the wall
 */
void InitABox(Game* game, int x, int y) {
    // Insert box position in the Game structure
    if (game->boxs.numberElt < MAX_CASE) {
        (*game).boxs.arrayElt[(*game).boxs.numberElt] = InitAMovable(x, y);
        (*game).boxs.numberElt++;
    }
}

/**
 * @brief Initialize a box position
 *
 * @param game Pointer on the game structure
 * @param x X position of the wall
 * @param y Y position of the wall
 */
void InitAContainer(Game* game, int x, int y) {
    if (game->containers.numberElt < MAX_CASE) {
        (*game).containers.arrayElt[(*game).containers.numberElt] = InitAnElement(x, y);
        (*game).containers.numberElt++;
    }
}

/**
 * @brief Read a map file and create game elements from this file
 *
 * @param game Pointer on the game structure
 * @param mapPath Path to the map file
 */
void InsertGameElementsFromMapFile(Game* game, char* mapPath) {
    int c;
    int x = 0;
    int y = 0;

    FILE* mapFile = fopen(mapPath, "r");

    // Check file openning
    if (mapFile == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    do {
        c = fgetc(mapFile);
        switch (c) {
        case '+':
            InitABox(game, x, y);
            x++;
            break;
        case '@':
            InitACharacter(game, x, y);
            x++;
            break;
        case '.':
            InitAContainer(game, x, y);
            x++;
            break;
        case '#':
            InitAWall(game, x, y);
            x++;
            break;
        case ' ':
            x++;
            break;
        case '\n':
            y++;
            x = 0;
            break;
        default:
            break;
        }

        if (x >= GAME_WIDTH)x = 0;
        if (y >= GAME_HEIGHT)y = 0;

    } while (c != EOF);

    fclose(mapFile);
}

Pos GetNextPosition(Pos position, Direction direction) {
    Pos nextPos;

    switch (direction) {
    case HAUT:
        nextPos.x = position.x;
        nextPos.y = position.y--;
        break;
    case DROITE:
        nextPos.x = position.x++;
        nextPos.y = position.y;
        break;
    case BAS:
        nextPos.x = position.x;
        nextPos.y = position.y++;
        break;
    case GAUCHE:
        nextPos.x = position.x--;
        nextPos.y = position.y;
        break;
    default:
        break;
    }

    return nextPos;
}

bool IsMovementAllowed(Game* game, Movable object) {
    Pos nextPos = GetNextPosition(object.pos, object.direction);

    //return CheckCollisionWithBorders(game, nextPos) && CheckCollisionWithWalls(game, nextPos);
    return true;
}

bool CheckCollisionWithBorders(Game* game, Pos nextPosition) {

    if ((nextPosition.x < 0) || (nextPosition.y < 0) || (nextPosition.x > game->gameWidth) || (nextPosition.y > game->gameHeight)) {
        return false;
    }
    else {
        return true;
    }
}

bool CheckCollisionWithWalls(Game* game, Pos nextPosition) {

    for (int i = 0; i < (*game).boxs.numberElt; i++) {
        if (nextPosition.x == (*game).walls.arrayElt[i].x && nextPosition.y == (*game).walls.arrayElt[i].y) {
            return false;
        }
    }
    return true;

}

void InitAGame(Game* game) {
    game->isInProgress = true;

    InitGameBorders(game, GAME_WIDTH, GAME_HEIGHT);
    InitEmptyGameElements(game);

    InsertGameElementsFromMapFile(game, MAP_PATH);
}
// //Fonction qui va permettre le non-dépassement des bordures du plateau pour les mouvements de la caisse
// static int overtaking_box(Game* game, WINDOW* win) {


//     switch (game->direction) {
//     case DROITE:
//         if (!(game->box.y >= game->right_limit - 1)) {
//             return 1;
//         }
//         else {
//             return 0;
//         }
//         break;

//     case BAS:
//         if (!(game->box.x >= game->bottom_limit - 1)) {
//             return 1;
//         }
//         else {
//             return 0;
//         }
//         break;

//     case GAUCHE:
//         if (!(game->box.y <= 1)) {
//             return 1;
//         }
//         else {
//             return 0;
//         }
//         break;

//     case HAUT:
//         if (!(game->box.x <= 1)) {
//             return 1;
//         }
//         else {
//             return 0;
//         }
//         break;

//     default:
//         return -1;
//         break;
//     }
// }

// //Fonction qui va permettre à ce que le perosnnage ne passe pas au dessus de la caisse si elle est contre un mur
// static int charac_recovery(Game* game, WINDOW* win) {



//     switch (game->direction) {
//     case DROITE:
//         if (!((game->box.y == game->right_limit - 1) && (game->box.x == game->character.x) && (game->character.y == game->right_limit - 2))) {
//             return 1;
//         }
//         else {
//             return 0;
//         }
//         break;

//     case GAUCHE:
//         if (!((game->box.y <= 1) && (game->box.x == game->character.x) && (game->character.y == 2))) {
//             return 1;
//         }
//         else {
//             return 0;
//         }
//         break;

//     case BAS:
//         if (!((game->box.x == game->bottom_limit - 1) && (game->box.y == game->character.y) && (game->character.x == game->bottom_limit - 2))) {
//             return 1;
//         }
//         else {
//             return 0;
//         }
//         break;

//     case HAUT:
//         if (!((game->box.x <= 1) && (game->box.y == game->character.y) && (game->character.x == 2))) {
//             return 1;
//         }
//         else {
//             return 0;
//         }
//         break;

//     default:
//         return -1;
//         break;
//     }
// }

void ResetAllDirections(Game* game) {
    for (int i = 0; i < game->characters.numberElt; i++) {
        game->characters.arrayElt[i].direction = -1;
    }
}


void UserInput(Game* game, int userId) {
    //on récupère le caractère tapé par l'utilisateur grâce à la fonction getch()
    char ch = getch(); //fonction contenue dans ncurses
    //ResetAllDirections(game);
    switch (ch) {
    case 'z':
        (*game).characters.arrayElt[userId].direction = HAUT;
        break;
    case 'd':
        (*game).characters.arrayElt[userId].direction = DROITE;
        break;
    case 's':
        (*game).characters.arrayElt[userId].direction = BAS;
        break;
    case 'q':
        (*game).characters.arrayElt[userId].direction = GAUCHE;
        break;
    case 'm':
        game->isInProgress = false;
        break;
    default:
        break;
    }

}

//La fonction qui exécute les mouvements du personnage
// static void move_charac(Game* game, WINDOW* win) {
//     if (game->direction == HAUT && (overtaking_charac(game, win)) && (charac_recovery(game, win))) {
//         game->character.x--;
//     }
//     else if (game->direction == GAUCHE && (overtaking_charac(game, win)) && (charac_recovery(game, win))) {
//         game->character.y--;
//     }
//     else if (game->direction == BAS && (overtaking_charac(game, win)) && (charac_recovery(game, win))) {
//         game->character.x++;
//     }
//     else if (game->direction == DROITE && (overtaking_charac(game, win)) && (charac_recovery(game, win))) {
//         game->character.y++;
//     }
// }

void MoveCharacter(Game* game, int userId) {
    //if (IsMovementAllowed(game, game->characters.arrayElt[userId])) {
    Pos pos = InitAnElement(2, 0);
    game->characters.arrayElt[userId].pos = pos;
    //GetNextPosition(game->characters.arrayElt[userId].pos, game->characters.arrayElt[userId].direction);
    //}
}

//La fonction qui va définir le mouvement d'une caisse poussée par le personnage 
// static void move_box(Game* game, WINDOW* win) {
//     printf("%d\n", game->direction);

//     //Mouvement de la caisse vers la droite 
//     if ((game->character.y == game->box.y) && (game->character.x == game->box.x) && (game->direction == DROITE) && (overtaking_box(game, win))) game->box.y++;

//     //Mouvement de la caisse vers la gauche
//     if ((game->character.y == game->box.y) && (game->character.x == game->box.x) && (game->direction == GAUCHE) && (overtaking_box(game, win))) game->box.y--;

//     //Mouvement de la caisse vers le bas 
//     if ((game->character.y == game->box.y) && (game->character.x == game->box.x) && (game->direction == BAS) && (overtaking_box(game, win))) game->box.x++;

//     //Mouvement de la caisse vers le haut
//     if ((game->character.y == game->box.y) && (game->character.x == game->box.x) && (game->direction == HAUT) && (overtaking_box(game, win))) game->box.x--;
// }


//Fonction affichage qui va afficher les éléments sur le plateau (personnage, murs, réceptacles, caisses)
void affichage(Game* game, WINDOW* win) {

    //Avant chaque affichage, on refresh pour mettre à jour le plateau et prendre en compte les mouvements définis par la fonction move_charac()
    werase(win);
    box(win, 0, 0);

    //On fait appel à la fonction qui s'occupe de l'affichage des murs du plateau 

    //Affichage d'une caisse
    for (int i = 0; i < game->boxs.numberElt;i++) {
        mvwprintw(win, (game->boxs.arrayElt[i].pos.y) + 1, (game->boxs.arrayElt[i].pos.x) + 1, "+");
    }

    //Affichage d'un receptacle
    for (int i = 0; i < game->containers.numberElt;i++) {
        mvwprintw(win, (game->containers.arrayElt[i].y) + 1, (game->containers.arrayElt[i].x) + 1, ".");
    }
    //Affichage d'un mur 
    for (int i = 0; i < game->walls.numberElt;i++) {
        mvwprintw(win, (game->walls.arrayElt[i].y) + 1, (game->walls.arrayElt[i].x) + 1, "#");
    }
    //Affichage d'un personnage
    for (int i = 0; i < game->characters.numberElt;i++) {
        mvwprintw(win, (game->characters.arrayElt[i].pos.y) + 1, (game->characters.arrayElt[i].pos.x) + 1, "@");
    }

    mvwprintw(win, 0, 0, "nb:%d x:%d y:%d", game->characters.numberElt, (game->characters.arrayElt[0].pos.x) + 1, (game->characters.arrayElt[0].pos.y) + 1);
    //Toujours refresh l'écran pour permettre l'affichage lorsqu'on utlise la biliothèque ncurses 
    wrefresh(win);
}


//Fonction execute qui va déterminer les conditions d'exécution du jeu 
void Execute(Game* game, WINDOW* win) {
    do { //faire

        affichage(game, win);
        //On fait appel à la fonction qui va récupérer la direction que l'utilisateur tape en fonction des touches q, z, s et d.

        UserInput(game, 0);

        //On fait appel à la fonction qui met à jour les coordonnées du personnage 
        MoveCharacter(game, 0);

        //On fait appel à la fonction qui va servir à faire bouger la caisse 
        //move_box(game, win);

        //On affiche le personnage, la caisse, le mur, le réceptacle
        affichage(game, win);

        //On fait appel à la fonction qui va permettre au personnage de ne pas passer au dessus de la caisse si elle est contre un mur 
        //charac_recovery(game, win);

        //On fait appel à la fonction ayant pour but de gérer le non dépassement des bordures du plateau par le personnage
        //overtaking_charac(game, win);

        //On fait appel à la fonction ayant pour but de gérer le non dépassement des bordures du plateau par la caisse
        //overtaking_box(game, win);

        //Temps entre chaque déplacement, pour laisser le temps au programme et au système 
        usleep(10000);
    } while (game->isInProgress); //tant que (*game).end est vrai = tant que la variable end ne devient pas fausse (voir ligne 65)
}


