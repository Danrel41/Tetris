#ifndef TESTS_S21_TESTS_H
#define TESTS_S21_TESTS_H

#define TEST_ARITHMETIC_OK 0
#define TEST_ARITHMETIC_BIG 1
#define TEST_ARITHMETIC_SMALL 2
#define TEST_ARITHMETIC_ZERO_DIV 3

#include <check.h>
#include <limits.h>

#include "../brick_game/tetris/backend.h"
#include "../brick_game/tetris/fsm.h"
#include "../brick_game/tetris/tetris.h"
#include "../gui/cli/frontend.h"

Suite *backend_fsm_test(void);

#endif  // TESTS_S21_TESTS_H
