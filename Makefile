CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
OBJ = parser.c transformations.c
OBJ_H = *.h
OBJ_TEST = tests.c
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	OPEN_CMD = xdg-open
	ADD_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURCE
endif

ifeq ($(UNAME_S),Darwin)
	OPEN_CMD = open
	ADD_LIB = -lcheck
endif

all: install

s21_3dviewer.a:
	@$(CC) $(CFLAGS) -c $(OBJ)
	@ar rcs $@ $(OBJ)
	@rm -rf *.o
	@ranlib s21_3dviewer.a

install:
	mkdir build
	mkdir 3DViewer_app
	cd build/ && qmake  ../3DViewer/3DViewer.pro && make
	cp -rf build/3DViewer.app 3DViewer_app
	make clean

run_app:
	./3DViewer_app/3DViewer.app/Contents/MacOS/3DViewer

uninstall:
	@rm -rf 3DViewer_app

dvi:
	@$(OPEN_CMD) dvi.md

dist:
	@mkdir dist
	@cp -R 3DViewer $(OBJ) $(OBJ_H) Makefile dvi.md dist
	@cd dist && tar cvzf 3DViewer_archive.tgz *
	@cd dist && rm -rf 3DViewer *.c *.h Makefile dvi.md
	@rm -rf 3DViewer_app

tests: clean
	@$(CC) $(CFLAGS) --coverage $(OBJ) $(OBJ_TEST) -o test $(ADD_LIB)
	@./test

gcov: tests
	@lcov -t "s21_test" -o fizzbuzz.info -c -d .
	@genhtml -o report fizzbuzz.info
	@$(OPEN_CMD) report/src/index.html

style:
	@clang-format -style=Google -n *.c *.h */*.h */*.cpp

clean:
	@-rm -rf *.o *.gcno *.gcda *.info *.gcov *.a *.dSYM test report build*

add_obj:
	@rm -rf obj
	git clone git@github.com:Momzh228/Weaver.git obj
	@rm -rf obj/.DS_Store obj/.git

.PHONY: install uninstall dvi dest tests gcov style clean
