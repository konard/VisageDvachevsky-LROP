#ifndef MENU_H
#define MENU_H

#include "utilities.h"

/* Main menu function - entry point for the menu system */
void run_main_menu(int* score, Difficulty* difficulty);

/* Internal menu functions */
void show_main_menu(void);
void run_games_menu(int* score, Difficulty difficulty);
void run_settings_menu(Difficulty* difficulty);
void run_difficulty_menu(Difficulty* difficulty);

#endif /* MENU_H */
