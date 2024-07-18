#include "backend.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../gui/cli/frontend.h"
#include "tetris.h"

void userInput(GameBas* game, bool hold) {
  if (hold) {
    switch (game->action) {
      case Left:
        game->player.new_x--;
        break;
      case Down:
        game->player.new_y++;
        game->fall_delay = game->info.speed;
        break;
      case Right:
        game->player.new_x++;
        break;
      case Action:
        if (game->player.rotate) rotationFigure(game);
        break;

      default:
        break;
    }
  }
}

bool control(int key, GameBas* game) {
  bool answer = true;

  if (key == ERR) {
    answer = false;
    game->action = NoKey;

  } else {
    if (key == KEY_UP) {
      game->action = Up;
    } else if (key == KEY_DOWN) {
      game->action = Down;
    } else if (key == KEY_LEFT) {
      game->action = Left;
    } else if (key == KEY_RIGHT) {
      game->action = Right;
    } else if (key == 'q') {
      game->action = Terminate;
    } else if (key == 'z') {
      game->action = Action;
    } else if (key == 'p') {
      game->action = Pause;
    } else if (key == 's') {
      game->action = Start;
    }
    game->fall_delay++;
  }
  return answer;
}

int** randomFigure(GameBas game) {
  srand(time(NULL));
  int rand_num = rand() % 7;

  return game.pattern[rand_num].figure;
}

int spawnFigure(GameBas* game) {
  int status = TET_MOVEMENT;

  game->player.cur_y = START_Y;
  game->player.cur_x = START_X;
  game->player.new_y = START_Y;
  game->player.new_x = START_X;

  game->player.rotate = true;
  if (SQUARE) game->player.rotate = false;

  copyArray(game->player.figure, game->info.next, FIGURES_SIZE, FIGURES_SIZE);
  copyArray(game->info.next, randomFigure(*game), FIGURES_SIZE, FIGURES_SIZE);

  if (collisionCheck(game, true, false)) status = TET_GAMEOVER;

  return status;
}

void fallingFigure(GameBas* game, bool* hold) {
  nanosleep(&game->time_delay, NULL);

  if (game->fall_delay-- <= 0) {
    game->action = Down;
    game->fall_delay = game->info.speed;
    *hold = true;
  }
}

void rotationFigure(GameBas* game) {
  int** another_array = {0};
  another_array = memAlloc(another_array, FIGURES_SIZE, FIGURES_SIZE);
  copyArray(another_array, game->player.figure, FIGURES_SIZE, FIGURES_SIZE);
  cleanPlayer(game->player);

  for (int i = 0, n = FIGURES_SIZE - 1; i < FIGURES_SIZE; i++, n--) {
    for (int j = 0; j < FIGURES_SIZE; j++) {
      game->player.figure[j][n] = another_array[i][j];
    }
  }

  if (collisionCheck(game, true, true)) {
    copyArray(game->player.figure, another_array, FIGURES_SIZE, FIGURES_SIZE);
  }

  freeArray(another_array, FIGURES_SIZE);
}

int collisionCheck(GameBas* game, bool hold, bool rotation) {
  int answer = 0;

  if (hold) {
    bool loop = true;

    for (int i = 0; i < FIGURES_SIZE && loop; i++) {
      for (int j = 0; j < FIGURES_SIZE && loop; j++) {
        if (game->player.figure[i][j] == 1 &&
            (FIELD_WALLS || FIELD_OBSTACLES)) {
          game->player.new_x = game->player.cur_x;
          loop = false;
          answer = 1;
        }

        if (!rotation && game->player.figure[i][j] == 1 &&
            (FIELD_FLOOR || FIELD_OBSTACLES)) {
          loop = false;
          answer = 2;
        }
      }
    }
  }

  return answer;
}

int statusCheck(GameBas* game, bool hold) {
  int status = TET_MOVEMENT;
  bool loop = true;

  if (hold && game->action == Pause) {
    attron(A_BLINK);
    mvprintw(2, 3, "PAUSE");
    status = TET_PAUSE;
    loop = false;
  }

  while (loop) {
    if (collisionCheck(game, hold, false) == 2) {
      status = TET_ATTACHINC;
    } else {
      cleanPlayer(game->player);
      game->player.cur_y = game->player.new_y;
      game->player.cur_x = game->player.new_x;
    }

    if (game->action != Up || status == TET_ATTACHINC)
      loop = false;
    else
      game->player.new_y++;
  }
  return status;
}

void saveField(Player player, GameInfo_t* info) {
  for (int i = 0; i < FIGURES_SIZE; i++) {
    for (int j = 0; j < FIGURES_SIZE; j++) {
      if (player.figure[i][j] == 1) {
        info->field FIGURE_POSITION = player.figure[i][j];
      }
    }
  }
}

void fullRow(GameBas* game) {
  int number_lines = 0;
  int** another_array = {0};
  another_array = memAlloc(another_array, FIELD_HEIGHT, FIELD_WIDTH);

  for (int i = FIELD_HEIGHT - 1, n = FIELD_HEIGHT - 1; i > -1; i--) {
    bool full_r = true;

    for (int j = FIELD_WIDTH - 1; j > -1; j--) {
      another_array[n][j] = game->info.field[i][j];

      if (game->info.field[i][j] == 0) {
        full_r = false;
      }
    }

    if (!full_r) {
      n--;
    } else {
      number_lines++;
    }
  }

  if (number_lines) {
    scoring(game, number_lines);
  }

  copyArray(game->info.field, another_array, FIELD_HEIGHT, FIELD_WIDTH);
  freeArray(another_array, FIELD_HEIGHT);
}

void cleanField(int** field) {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      field[i][j] = 0;
    }
  }
}

void scoring(GameBas* game, int number_lines) {
  if (number_lines == 1) {
    game->info.score += 100;
  } else if (number_lines == 2) {
    game->info.score += 300;
  } else if (number_lines == 3) {
    game->info.score += 700;
  } else if (number_lines == 4) {
    game->info.score += 1500;
  }

  if (game->info.score > game->info.high_score) {
    game->info.high_score = game->info.score;
  }

  while (game->info.level < 10 &&
         game->info.score >= game->info.level * NUMBER_NEW_LEVEL) {
    game->info.level++;
    game->info.speed -= 5;
    game->fall_delay = game->info.speed;
  }
}
