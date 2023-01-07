
#include <stdio.h> 
#include <stdlib.h>
#include "game.h"

//Création de la fenêtre de jeu
WINDOW* create_newwin(int h, int w, int y, int x) {

  //On initialise une nouvelle fenetre : 
  WINDOW* local_win;
  local_win = newwin(h, w, y, x);

  //On dessine les bordures autour de notre nouvelle fenetre grace a la fonction box : 
  box(local_win, 0, 0);
  //
  wrefresh(local_win);
  return local_win;
}



int main(int argc, char* argv[]) {
  WINDOW* win = NULL;
  //On déclare un variable de type game pour déclarer un nouveau jeu 
  Game    game;
  //Initialisation de la hauteur et de la largeur de la fenetre : 
  int     h = 42;
  int     w = 82;

  initscr();
  //cbreak();
  curs_set(false);
  refresh();
  //On crée une nouvelle fenêtre en fonction de la hauteur, de la largeur et on fait en sorte de la placer au centre du terminal (calcul)
  win = create_newwin(h, w, (LINES - h) / 2, (COLS - w) / 2);
  //Keypad récupère une frappe au clavier 
  keypad(win, true);
  /*On fait appel à la fonction nodelay() dans ncurses pour permettre de rendre notre fonction getch() non bloquante. Getch() va nous permettre de récupérer
  ce que l'utilisateur va taper sur le clavier pour donner la direction au personnage*/
  nodelay(win, true);
  //On appelle notre fonction constructeur qui contient toutes les variables que nous allons utiliser en fonction de notre instance Game, de la hauteur et de la longueur
  InitAGame(&game);
  //Une fois cela fait, on peut lancer la fonction qui va démarrer le jeu en fonction de l'instance Game et de la fenêtre de jeu créée 
  Execute(&game, win);

  //nodelay(win, true);
  //wattron(win, A_REVERSE);

  //delwin(win);
  endwin();
  return 0;
}
