#ifndef GAMES_H
#define GAMES_H

#include "utilities.h"

/* Game function prototypes
 * Each game returns:
 *   1 - victory
 *   0 - defeat
 *  -1 - early exit (optional)
 */

int start_lucky_game(Difficulty difficulty);
int start_find_game(Difficulty difficulty);
int start_minesweeper_game(Difficulty difficulty);
int start_tictactoe_game(Difficulty difficulty);
int start_tetris_game(Difficulty difficulty);

#endif /* GAMES_H */
