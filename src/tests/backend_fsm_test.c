#include "ncurses.h"
#include "stdio.h"
#include "tests.h"

START_TEST(randomFigure_test) {
  GameBas game = initGame();

  for (int test = 0; test < 10; test++) {
    int **array = randomFigure(game);

    int n = 0;
    int cycle = 1;

    for (int i = 0; i < FIGURES_SIZE && cycle; i++) {
      for (int j = 0; j < FIGURES_SIZE && cycle; j++) {
        if (array[i][j] != game.pattern[n].figure[i][j]) {
          i = 0;
          j = -1;
          n++;
        }
        if (n >= QUANTITY_FIGURES) {
          cycle = 0;
        }
      }
    }
    ck_assert_int_eq(cycle, 1);
  }

  freeGame(&game);
}

END_TEST

START_TEST(copyArray_test) {
  GameBas game = initGame();
  int **array = {0};
  array = memAlloc(array, FIGURES_SIZE, FIGURES_SIZE);

  copyArray(array, game.pattern[5].figure, FIGURES_SIZE, FIGURES_SIZE);

  int cycle = 1;
  for (int i = 0; i < FIGURES_SIZE && cycle; i++) {
    for (int j = 0; j < FIGURES_SIZE && cycle; j++) {
      if (array[i][j] != game.pattern[5].figure[i][j]) {
        cycle = 0;
      }
    }
  }
  ck_assert_int_eq(cycle, 1);

  freeGame(&game);
  freeArray(array, FIGURES_SIZE);
}

END_TEST

START_TEST(collisionCheck_test) {
  GameBas game = initGame();
  spawnFigure(&game);

  int answer = 0;

  answer = collisionCheck(&game, true, false);
  ck_assert_int_eq(answer, 0);

  game.player.new_x = 12;

  answer = collisionCheck(&game, true, false);
  ck_assert_int_eq(answer, 1);

  game.player.new_y = 30;

  answer = collisionCheck(&game, true, false);
  ck_assert_int_eq(answer, 2);

  freeGame(&game);
}

END_TEST

START_TEST(saveField_test) {
  GameBas game = initGame();

  saveField(game.player, &game.info);

  int cycle = 1;
  int bloc = 0;

  for (int i = 0; i < FIELD_HEIGHT && cycle; i++) {
    for (int j = 0; j < FIELD_WIDTH && cycle; j++) {
      if (game.info.field[i][j] == 1) {
        bloc++;
      }
      if (bloc > 4) {
        cycle = 0;
      }
    }
  }

  if (bloc != 4) {
    cycle = 0;
  }

  ck_assert_int_eq(cycle, 0);

  freeGame(&game);
}

END_TEST

START_TEST(spawnFigure_test) {
  GameBas game = initGame();

  game.player.cur_x = 9;
  game.player.cur_y = 19;
  game.player.new_x = 9;
  game.player.new_y = 19;

  copyArray(game.info.next, game.pattern[5].figure, FIGURES_SIZE, FIGURES_SIZE);

  spawnFigure(&game);

  ck_assert_int_eq(game.player.cur_x, START_X);
  ck_assert_int_eq(game.player.cur_y, START_Y);
  ck_assert_int_eq(game.player.new_x, START_X);
  ck_assert_int_eq(game.player.new_y, START_Y);

  int cycle = 1;
  for (int i = 0; i < FIGURES_SIZE && cycle; i++) {
    for (int j = 0; j < FIGURES_SIZE && cycle; j++) {
      if (game.player.figure[i][j] != game.pattern[5].figure[i][j]) {
        cycle = 0;
      }
    }
  }

  ck_assert_int_eq(cycle, 1);

  freeGame(&game);
}

END_TEST

START_TEST(fullRow_test) {
  GameBas game = initGame();

  for (int i = 0; i < FIELD_WIDTH; i++) {
    game.info.field[19][i] = 1;
  }

  fullRow(&game);

  ck_assert_int_eq(game.info.score, 100);

  freeGame(&game);
}

END_TEST

START_TEST(scoring_test) {
  GameBas game = initGame();

  scoring(&game, 2);
  ck_assert_int_eq(game.info.score, 300);

  scoring(&game, 3);
  ck_assert_int_eq(game.info.score, 1000);

  scoring(&game, 4);
  ck_assert_int_eq(game.info.score, 2500);

  ck_assert_int_eq(game.info.level, 5);

  freeGame(&game);
}

END_TEST

START_TEST(rotationFigure_test) {
  GameBas game = initGame();

  copyArray(game.player.figure, game.pattern[0].figure, FIGURES_SIZE,
            FIGURES_SIZE);

  rotationFigure(&game);

  int cycle = 1;
  for (int i = 0, n = FIGURES_SIZE - 1; i < FIGURES_SIZE && cycle; i++, n--) {
    for (int j = 0; j < FIGURES_SIZE && cycle; j++) {
      if (game.player.figure[j][n] != game.pattern[0].figure[i][j]) {
        cycle = 0;
      }
    }
  }
  ck_assert_int_eq(cycle, 1);

  freeGame(&game);
}

END_TEST

START_TEST(statusCheck_test) {
  GameBas game = initGame();

  game.action = Pause;
  ck_assert_int_eq(statusCheck(&game, true), TET_PAUSE);

  ck_assert_int_eq(statusCheck(&game, false), TET_MOVEMENT);

  freeGame(&game);
}

END_TEST

START_TEST(control_test) {
  GameBas game = initGame();

  control(KEY_UP, &game);
  ck_assert_int_eq(game.action, Up);
  userInput(&game, true);

  control(KEY_DOWN, &game);
  ck_assert_int_eq(game.action, Down);
  userInput(&game, true);

  control(KEY_LEFT, &game);
  ck_assert_int_eq(game.action, Left);
  userInput(&game, true);

  control(KEY_RIGHT, &game);
  ck_assert_int_eq(game.action, Right);
  userInput(&game, true);

  control('q', &game);
  ck_assert_int_eq(game.action, Terminate);
  userInput(&game, true);

  control('z', &game);
  ck_assert_int_eq(game.action, Action);
  userInput(&game, true);

  control('p', &game);
  ck_assert_int_eq(game.action, Pause);
  userInput(&game, true);

  control('s', &game);
  ck_assert_int_eq(game.action, Start);
  userInput(&game, true);

  control(ERR, &game);
  ck_assert_int_eq(game.action, NoKey);
  userInput(&game, true);

  freeGame(&game);
}

END_TEST

START_TEST(fallingFigure_test) {
  GameBas game = initGame();

  bool hold = false;

  int i = 0;
  for (; !hold; i++) {
    fallingFigure(&game, &hold);
  }

  ck_assert_int_eq(i, 71);

  freeGame(&game);
}

END_TEST

START_TEST(cleanField_test) {
  GameBas game = initGame();

  int count = 0;

  for (int s = 0; s < 2; s++) {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
      for (int j = 0; j < FIELD_WIDTH; j++) {
        if (s == 0) {
          game.info.field[i][j] = 1;
          count++;
        } else if (s == 1) {
          cleanField(game.info.field);
          count = 0;
        }
      }
    }
    if (s == 0)
      ck_assert_int_eq(200, count);
    else
      ck_assert_int_eq(0, count);
  }

  freeGame(&game);
}

END_TEST

START_TEST(saveRecord_test) {
  GameBas game = initGame();
  saveRecord(99999);
  freeGame(&game);

  game = initGame();

  ck_assert_int_eq(game.info.high_score, 99999);
  saveRecord(0);
  freeGame(&game);
}

END_TEST

Suite *backend_fsm_test() {
  Suite *s;
  TCase *tc_randomFigure_test, *tc_copyArray_test, *tc_collisionCheck_test,
      *tc_saveField_test, *tc_spawnFigure_test, *tc_fullRow_test,
      *tc_scoring_test, *tc_rotationFigure_test, *tc_statusCheck_test,
      *tc_control_test, *tc_fallingFigure_test, *tc_cleanField_test,
      *tc_saveRecord_test;

  s = suite_create("backend_fsm_test");

  tc_randomFigure_test = tcase_create("randomFigure_test");
  tcase_add_test(tc_randomFigure_test, randomFigure_test);
  suite_add_tcase(s, tc_randomFigure_test);

  tc_copyArray_test = tcase_create("copyArray_test");
  tcase_add_test(tc_copyArray_test, copyArray_test);
  suite_add_tcase(s, tc_copyArray_test);

  tc_collisionCheck_test = tcase_create("collisionCheck_test");
  tcase_add_test(tc_collisionCheck_test, collisionCheck_test);
  suite_add_tcase(s, tc_collisionCheck_test);

  tc_saveField_test = tcase_create("saveField_test");
  tcase_add_test(tc_saveField_test, saveField_test);
  suite_add_tcase(s, tc_saveField_test);

  tc_spawnFigure_test = tcase_create("spawnFigure_test");
  tcase_add_test(tc_spawnFigure_test, spawnFigure_test);
  suite_add_tcase(s, tc_spawnFigure_test);

  tc_fullRow_test = tcase_create("fullRow_test");
  tcase_add_test(tc_fullRow_test, fullRow_test);
  suite_add_tcase(s, tc_fullRow_test);

  tc_scoring_test = tcase_create("scoring_test");
  tcase_add_test(tc_scoring_test, scoring_test);
  suite_add_tcase(s, tc_scoring_test);

  tc_rotationFigure_test = tcase_create("rotationFigure_test");
  tcase_add_test(tc_rotationFigure_test, rotationFigure_test);
  suite_add_tcase(s, tc_rotationFigure_test);

  tc_statusCheck_test = tcase_create("statusCheck_test");
  tcase_add_test(tc_statusCheck_test, statusCheck_test);
  suite_add_tcase(s, tc_statusCheck_test);

  tc_control_test = tcase_create("control_test");
  tcase_add_test(tc_control_test, control_test);
  suite_add_tcase(s, tc_control_test);

  tc_fallingFigure_test = tcase_create("fallingFigure_test");
  tcase_add_test(tc_fallingFigure_test, fallingFigure_test);
  suite_add_tcase(s, tc_fallingFigure_test);

  tc_cleanField_test = tcase_create("cleanField_test");
  tcase_add_test(tc_cleanField_test, cleanField_test);
  suite_add_tcase(s, tc_cleanField_test);

  tc_saveRecord_test = tcase_create("saveRecord_test");
  tcase_add_test(tc_saveRecord_test, saveRecord_test);
  suite_add_tcase(s, tc_saveRecord_test);

  return s;
}
