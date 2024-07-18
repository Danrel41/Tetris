#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>

#include "../../brick_game/tetris/fsm.h"
#include "../../brick_game/tetris/tetris.h"

void screenGame();

void wallPrint(int y1, int x1, int y2, int x2);
void printInterface();
void introGame();
void printGameover();

void printInfo(GameInfo_t info);

void printFigure(int **figure, int rows, int cols, int x, int y, char symbol,
                 bool aura);
void printPlayer(Player player);
void cleanPlayer(Player player);

#endif  // FRONTEND_H