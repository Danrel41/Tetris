#include "frontend.h"

void screenGame() {
  initscr();
  curs_set(0);
  noecho();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
}

void wallPrint(int y1, int x1, int y2, int x2) {
  for (int i = 0; i < x2 - x1 + 1; i++) {
    mvprintw(y1, x1 + i, WALL_SYMBOL);
    mvprintw(y2, x1 + i, WALL_SYMBOL);
  }
  for (int i = 0; i < y2 - y1; i++) {
    mvprintw(y1 + i, x1, WALL_SYMBOL);
    mvprintw(y1 + i, x2, WALL_SYMBOL);
  }
}

void printInterface() {
  wallPrint(0, 0, 21, 11);
  wallPrint(0, 0, 21, 19);

  mvprintw(2, 14, "NEX");
  mvprintw(9, 14, "REC");
  mvprintw(12, 14, "SCR");
  mvprintw(15, 14, "LVL");
  mvprintw(18, 13, "TET[]");
  mvprintw(19, 13, "[]RIS");

  refresh();
}

void introGame() {
  mvprintw(2, 2, "IF YOUR");
  mvprintw(3, 2, "WANT TO");
  mvprintw(4, 2, "START");
  mvprintw(5, 2, "PRESS:");
  mvprintw(8, 2, "s-START");
  mvprintw(10, 2, "q-EXIT");
}

void printGameover() {
  wallPrint(5, 0, 14, 11);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 10; j++) {
      mvprintw(6 + i, 1 + j, " ");
    }
  }
  mvprintw(7, 2, "GAMEOVER");
  mvprintw(8, 2, "RESTART?");
  mvprintw(10, 2, "s-REST");
  mvprintw(12, 2, "q-EXIT");
}

void printInfo(GameInfo_t info) {
  printFigure(info.field, FIELD_HEIGHT, FIELD_WIDTH, 1, 1, GAME_SYMBOL, true);
  printFigure(info.next, FIGURES_SIZE, FIGURES_SIZE, 4, 13, GAME_SYMBOL, true);

  mvprintw(10, 13, "%5d", info.high_score);
  mvprintw(13, 13, "%5d", info.score);
  mvprintw(16, 13, "%5d", info.level);

  refresh();
}

void printFigure(int** figure, int rows, int cols, int y, int x, char symbol,
                 bool aura) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (figure[i][j] == 1) {
        mvprintw(y + i, x + j, "%c", symbol);
      } else if (aura)
        mvprintw(y + i, x + j, "%c", ' ');
    }
  }

  refresh();
}

void printPlayer(Player player) {
  printFigure(player.figure, FIGURES_SIZE, FIGURES_SIZE, player.new_y,
              player.new_x, GAME_SYMBOL, false);
}

void cleanPlayer(Player player) {
  printFigure(player.figure, FIGURES_SIZE, FIGURES_SIZE, player.cur_y,
              player.cur_x, ' ', false);
}
