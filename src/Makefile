CC=gcc

ifeq ($(shell uname), Darwin)
	LIBS = $(shell pkg-config --libs check)
else
	LIBS = -lcheck -lsubunit -lrt -lm -lpthread
endif

FLAGS =-c -Wall -Werror -Wextra -std=c11 -lncurses

CFLAGS =-c -Wall -Werror -Wextra -std=c11 -lncurses $(LIBS)

all: gcov_report install dvi

game.a:
	@rm -f libgame.a
	@mkdir object
	@$(CC) $(FLAGS) brick_game/tetris/*.c gui/cli/*.c
	@mv *.o object

	@ar rc libgame.a object/*.o
	@ranlib libgame.a

install: game.a
	@gcc -o Tetris main.c -L. -lgame -lncurses

uninstall:
	@rm -rf Tetris record.txt

dvi:
	@mkdir tetinfo
	@latex tetinfo.tex
	@mv *.log *.dvi *.aux tetinfo/
	@evince tetinfo/tetinfo.dvi
	

dist:
	@tar czvf dist.tar.gz *

test: gcov_libgame.a
	@$(CC) $(CFLAGS) tests/*.c -lncurses 
	@$(CC) --coverage *.o libgame.a $(LIBS) -lncurses -o tetris_test
	@./tetris_test

valgrind: test
	@valgrind --tool=memcheck --leak-check=yes --track-origins=yes -s ./tetris_test 

gcov_libgame.a:
	@rm -f libgame.a
	@$(CC) --coverage $(CFLAGS) -lncurses brick_game/tetris/*.c gui/cli/*.c
	@ar rc libgame.a *.o
	@ranlib libgame.a

gcov_report: test
	@lcov -t "gcov_report" -o Coverage_Report.info -c -d .
	@genhtml -o ./report Coverage_Report.info
	@open ./report/index.html
	@mv *.info *.gcno *.gcda report/

clean:
	@rm -rf *.o *_test *.gcda *.gcno *.css *.html *.gcov *.a *.info *.aux *.log *.dvi report
	@rm -rf object tetinfo
	@rm -rf *.gz

rebuild: clean all