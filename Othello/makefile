CXX=g++
CFLAGS = -std=c++11

.PHONY: run clean

all: othello

othello: gameboard.o game.o
		${CXX} ${CFLAGS} gameboard.o game.o -o $@

game.o:	game.cpp board.h
		${CXX} ${CFLAGS} -c $<

gameboard.o: gameboard.cpp board.h
		${CXX} ${CFLAGS} -c $<

run: othello
		./othello

clean:
		rm  *.o othello
