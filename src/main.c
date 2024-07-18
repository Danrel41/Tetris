#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "brick_game/tetris/backend.h"
#include "brick_game/tetris/fsm.h"
#include "brick_game/tetris/tetris.h"
#include "gui/cli/frontend.h"

int main() {
  GameBas game = initGame();

  screenGame();
  printInterface();

  bool hold = true;

  introGame();
  while (game.action != Terminate) {
    hold = control(getch(), &game);

    switch (game.status) {
      case TET_START:
        if (game.action == Start) {
          printInfo(game.info);
          game.status = TET_SPAWN;
        }
        break;
      case TET_SPAWN:
        game.status = spawnFigure(&game);
        printInfo(game.info);
        break;
      case TET_MOVEMENT:
        printPlayer(game.player);
        fallingFigure(&game, &hold);
        userInput(&game, hold);
        game.status = statusCheck(&game, hold);
        break;
      case TET_PAUSE:
        if (game.action == Pause) {
          attroff(A_BLINK);
          mvprintw(2, 3, "     ");
          game.status = TET_MOVEMENT;
        }
        break;
      case TET_ATTACHINC:
        saveField(game.player, &game.info);
        fullRow(&game);
        game.status = TET_SPAWN;
        break;
      case TET_GAMEOVER:
        printGameover();

        if (game.action == Start) {
          game.info.score = 0;
          game.info.level = 1;
          game.info.speed = START_SPEED;
          cleanField(game.info.field);
          printInfo(game.info);
          game.status = TET_MOVEMENT;
        }
        break;
    }
  }

  saveRecord(game.info.high_score);

  endwin();

  freeGame(&game);

  return 0;
}
