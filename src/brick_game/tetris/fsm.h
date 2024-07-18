#ifndef FSM_H
#define FSM_H

#include "stdbool.h"
#include "tetris.h"
#include "time.h"

typedef struct GameInfo_t {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  bool pause;
} GameInfo_t;

typedef enum Statuses {
  TET_START,
  TET_MOVEMENT,
  TET_ATTACHINC,
  TET_SPAWN,
  TET_PAUSE,
  TET_GAMEOVER
} Statuses;

typedef struct FiguresPattern {
  int **figure;
} FiguresPattern;

typedef struct {
  int **figure;
  int cur_y;
  int cur_x;
  int new_y;
  int new_x;
  bool rotate;
} Player;

typedef enum UserAction_t {
  NoKey,
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct GameBas {
  GameInfo_t info;
  Statuses status;
  FiguresPattern *pattern;
  Player player;
  UserAction_t action;
  struct timespec time_delay;
  int fall_delay;
} GameBas;

int **memAlloc(int **array, int rows, int cols);
void copyArray(int **from_array, int **to_array, int rows, int cols);
void freeArray(int **array, int rows);

GameBas initGame();
void initGameInfo(GameBas *game);
void initFigures(GameBas *game);
void initPlayer(GameBas *game);
void freeGame(GameBas *game);

int reading_file();
void saveRecord(int score);

#endif  // FSM_H