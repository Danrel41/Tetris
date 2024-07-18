#ifndef TETRIS_H
#define TETRIS_H

#include <time.h>

extern int nanosleep(const struct timespec *req, struct timespec *rem);

#define FIELD_HEIGHT 20
#define FIELD_WIDTH 10
#define QUANTITY_FIGURES 7
#define FIGURES_SIZE 5
#define START_Y 1
#define START_X 3

#define GAME_SYMBOL 'H'
#define WALL_SYMBOL "*"

#define DELAY_SECONDS 0
#define DELAY_NANOSECONDS 10000000
#define NUMBER_NEW_LEVEL 600
#define START_SPEED 70

#define NAME_FILE "record.txt"

#undef ERR
#define ERR (-1)

#undef OK
#define OK (0)

#define FIELD_WALLS game->player.new_x + j >= FIELD_WIDTH + 1 || game->player.new_x + j <= 0
#define FIELD_FLOOR game->player.new_y + i >= FIELD_HEIGHT + 1
#define FIELD_OBSTACLES \
  game->info.field[(game->player.new_y + i - 1) % FIELD_HEIGHT][game->player.new_x + j - 1]

#define FIGURE_POSITION [player.cur_y + i - 1][player.cur_x + j - 1]

#define SQUARE game->info.next[1][1] && game->info.next[1][2] && game->info.next[2][1]

#define INIT_ARRAY(array, el_0_0, el_0_1, el_0_2, el_0_3, el_0_4, el_1_0, el_1_1, el_1_2, el_1_3,  \
                   el_1_4, el_2_0, el_2_1, el_2_2, el_2_3, el_2_4, el_3_0, el_3_1, el_3_2, el_3_3, \
                   el_3_4, el_4_0, el_4_1, el_4_2, el_4_3, el_4_4)                                 \
  array[0][0] = el_0_0;                                                                            \
  array[0][1] = el_0_1;                                                                            \
  array[0][2] = el_0_2;                                                                            \
  array[0][3] = el_0_3;                                                                            \
  array[0][4] = el_0_4;                                                                            \
  array[1][0] = el_1_0;                                                                            \
  array[1][1] = el_1_1;                                                                            \
  array[1][2] = el_1_2;                                                                            \
  array[1][3] = el_1_3;                                                                            \
  array[1][4] = el_1_4;                                                                            \
  array[2][0] = el_2_0;                                                                            \
  array[2][1] = el_2_1;                                                                            \
  array[2][2] = el_2_2;                                                                            \
  array[2][3] = el_2_3;                                                                            \
  array[2][4] = el_2_4;                                                                            \
  array[3][0] = el_3_0;                                                                            \
  array[3][1] = el_3_1;                                                                            \
  array[3][2] = el_3_2;                                                                            \
  array[3][3] = el_3_3;                                                                            \
  array[3][4] = el_3_4;                                                                            \
  array[4][0] = el_4_0;                                                                            \
  array[4][1] = el_4_1;                                                                            \
  array[4][2] = el_4_2;                                                                            \
  array[4][3] = el_4_3;                                                                            \
  array[4][4] = el_4_4;

#endif  // TETRIS_H