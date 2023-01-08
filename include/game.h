#include<ncurses.h>
#include<stdio.h>
#include <pthread.h>

#define GAME_WIDTH 60
#define GAME_HEIGHT 40
#define INFO_HEIGHT 20

#define MAX_CASE (GAME_WIDTH * GAME_HEIGHT) +2
#define MAX_PLAYER 3

#define MAP_PATH "./map"

/**
 * @brief Direction enumeration
 *
 */
typedef enum { HAUT, DROITE, BAS, GAUCHE }  Direction;

/**
 * @brief Position structure
 *
 */
typedef struct {
  int x;
  int y;
} Pos;

/**
 * @brief Position structure
 *
 */
typedef struct {
  Pos pos;
  Direction direction;
} Movable;

/**
 * @brief Elements structure
 *
 */
typedef struct {
  Movable arrayElt[MAX_CASE];
  int numberElt;
} Movables;

/**
 * @brief Elements structure
 *
 */
typedef struct {
  Pos arrayElt[MAX_CASE];
  int numberElt;
} Elements;


/**
 * @brief Game Structure
 *
 */
typedef struct {

  bool isInProgress;

  int gameWidth;
  int gameHeight;

  Movables characters;
  Movables boxs;
  Elements walls;
  Elements containers;

  pthread_mutex_t gameLocker;
  pthread_cond_t gameUpdate;
} Game;

void InitGameBorders(Game* game, int width, int height);
Pos InitAnElement(int x, int y);
Movable InitAMovable(int x, int y);
void InitEmptyGameElements(Game* game);
void InitACharacter(Game* game, int x, int y);
void InitAWall(Game* game, int x, int y);
void InitABox(Game* game, int x, int y);
void InitAContainer(Game* game, int x, int y);
void InsertGameElementsFromMapFile(Game* game, char* mapPath);
Pos GetNextPosition(Pos position, Direction direction);
bool IsMovementAllowed(Game* game, Movable object);
bool CheckCollisionWithBorders(Game* game, Pos nextPosition);
bool CheckCollisionWithWalls(Game* game, Pos nextPosition);
void InitAGame(Game* game);
void ResetAllDirections(Game* game);
void UserInput(Game* game, int userId);
void MoveCharacter(Game* game, int userId);
void affichage(Game* game, WINDOW* win);
void Execute(Game* game, WINDOW* win);
