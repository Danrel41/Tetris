#ifndef BACKEND_H
#define BACKEND_H

#include <stdbool.h>
#include <time.h>

#include "fsm.h"

void userInput(GameBas *game, bool hold);
bool control(int key, GameBas *game);

int **randomFigure(GameBas game);

int spawnFigure(GameBas *game);
void fallingFigure(GameBas *game, bool *hold);
void rotationFigure(GameBas *game);
int collisionCheck(GameBas *game, bool hold, bool rotation);
int statusCheck(GameBas *game, bool hold);

void saveField(Player player, GameInfo_t *info);
void fullRow(GameBas *game);
void cleanField(int **field);
void scoring(GameBas *game, int number_lines);

#endif  // BACKEND_H