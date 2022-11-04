CC = g++ -std=c++17
CFLAGS = -Wall -Wextra -Werror
GTEST = -lgtest

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	LFLAGS = -lpthread
	OPEN = -xdg-open
endif

all : huffman

huffman : main.o interface.o archiver.o huffman_tree.o
	$(CC) $^ -o $@ $(LFLAGS)

tests : test

test : test.o archiver.o huffman_tree.o
	$(CC) $^ -o $@ $(GTEST) $(LFLAGS)
	./test

mem : test
ifeq ($(OS), Linux)
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./test
else
	CK_FORK=no leaks --atExit -- ./test
endif

main.o : main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.o : %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -rf *.o test huffman

rebuild : clean all

cppcheck :
	cppcheck --std=c++17 --language=c++ --enable=all --suppress=missingIncludeSystem --suppress=missingInclude --suppress=unmatchedSuppression *.cpp *.h

style :
	@clang-format -n --Werror *.h --style=Google --ferror-limit=3
	@clang-format -n --Werror *.cpp --style=Google --ferror-limit=3
