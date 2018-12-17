BIN = ./
SRC = ./src
OBJ = ./obj

FILES = $(OBJ)/cigar.o \
		$(OBJ)/util.o

CC = g++

CFLAGS = -c -I$(SRC) -std=c++11 -O3
LFLAGS =

all: $(BIN)/bwt

$(BIN)/bwt: $(FILES) $(OBJ)/main.o
	$(CC) $(LFLAGS) -o $@ $(FILES) $(OBJ)/main.o -lm

$(OBJ)/main.o: $(SRC)/main.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/main.o $(SRC)/main.cpp

$(OBJ)/bwt.o: $(SRC)/bwt.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/bwt.o $(SRC)/bwt.cpp

$(OBJ)/util.o: $(SRC)/util.cpp $(SRC)/util.h
	$(CC) $(CFLAGS) -o $(OBJ)/util.o $(SRC)/util.cpp

$(OBJ)/cigar.o: $(SRC)/cigar.cpp $(SRC)/cigar.h
	$(CC) $(CFLAGS) -o $(OBJ)/cigar.o $(SRC)/cigar.cpp

clean:
	(rm $(OBJ)/* ./bwt | true)
