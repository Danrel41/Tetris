#include "fsm.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "backend.h"
#include "tetris.h"

int** memAlloc(int** array, int rows, int cols) {
  array = (int**)calloc(rows, sizeof(int*));
  if (array) {
    for (int i = 0; i < rows; i++) {
      array[i] = (int*)calloc(cols, sizeof(int));
    }
  }
  return array;
}

void copyArray(int** to_array, int** from_array, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      to_array[i][j] = from_array[i][j];
    }
  }
}

void freeArray(int** array, int rows) {
  for (int i = 0; i < rows; i++) {
    free(array[i]);
  }
  free(array);
}

GameBas initGame() {
  GameBas game = {};

  game.pattern = (FiguresPattern*)calloc(QUANTITY_FIGURES, sizeof(int**));
  game.action = Start;

  Statuses status = 0;
  game.status = status;

  struct timespec time_delay = {DELAY_SECONDS, DELAY_NANOSECONDS};
  game.time_delay = time_delay;

  initFigures(&game);
  initGameInfo(&game);
  initPlayer(&game);

  copyArray(game.info.next, randomFigure(game), FIGURES_SIZE, FIGURES_SIZE);

  game.fall_delay = START_SPEED;

  return game;
}

void initGameInfo(GameBas* game) {
  GameInfo_t info = {};

  info.field = memAlloc(info.field, FIELD_HEIGHT, FIELD_WIDTH);
  info.next = memAlloc(info.next, FIGURES_SIZE, FIGURES_SIZE);

  info.score = 0;
  info.high_score = reading_file();
  info.level = 1;
  info.speed = START_SPEED;
  info.pause = false;

  game->info = info;
}

void initFigures(GameBas* game) {
  for (int i = 0; i < QUANTITY_FIGURES; i++) {
    FiguresPattern fig = {};
    fig.figure = memAlloc(fig.figure, FIGURES_SIZE, FIGURES_SIZE);

    if (i == 0) {
      INIT_ARRAY(fig.figure, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0,
                 1, 0, 0, 0, 0, 0, 0, 0);  // [][][][]
    } else if (i == 1) {
      INIT_ARRAY(fig.figure, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0);  // []
                                           // [][][]
    } else if (i == 2) {
      INIT_ARRAY(fig.figure, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0);  //     []
                                           // [][][]
    } else if (i == 3) {
      INIT_ARRAY(fig.figure, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0);  // [][]
                                           //   [][]
    } else if (i == 4) {
      INIT_ARRAY(fig.figure, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0);  //   [][]
                                           // [][]
    } else if (i == 5) {
      INIT_ARRAY(fig.figure, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0);  //   []
                                           // [][][]
    } else if (i == 6) {
      INIT_ARRAY(fig.figure, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
                 1, 1, 0, 0, 0, 0, 0, 0);  // [][]
                                           //   [][]
    }
    game->pattern[i] = fig;
  }
}

void initPlayer(GameBas* game) {
  Player player = {};

  player.figure = memAlloc(player.figure, FIGURES_SIZE, FIGURES_SIZE);
  player.cur_y = START_Y;
  player.cur_x = START_X;
  player.new_y = START_Y;
  player.new_x = START_X;
  player.rotate = true;

  game->player = player;
}

void freeGame(GameBas* game) {
  freeArray(game->info.field, FIELD_HEIGHT);
  freeArray(game->player.figure, FIGURES_SIZE);
  freeArray(game->info.next, FIGURES_SIZE);

  for (int i = 0; i < QUANTITY_FIGURES; i++) {
    freeArray(game->pattern[i].figure, FIGURES_SIZE);
  }

  free(game->pattern);
}

int reading_file() {
  int answer = 0;

  FILE* file = fopen(NAME_FILE, "r");
  if (file != NULL) {
    while (fscanf(file, "%d", &answer) != EOF) {
    }

    fclose(file);
  }

  return answer;
}

void saveRecord(int score) {
  if (score > reading_file()) {
    FILE* file = fopen(NAME_FILE, "w");
    fprintf(file, "%d\n", score);

    fclose(file);
  }
}
