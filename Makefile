OUT=bin/
OBJ=$(OUT)obj/
SRC=src/
INC=$(SRC)includes/

CC=gcc -std=gnu99

CCFLAGS=-O2 -Wall -Wextra
LDFLAGS=

$(OUT)kyanite: $(OBJ)kyanite.o $(OBJ)kylex.o
	$(CC) -o $(OUT)kyanite $(OBJ)kyanite.o $(OBJ)kylex.o $(LDFLAGS)

$(OBJ)kyanite.o: $(SRC)kyanite.c $(INC)kyanite.h $(INC)kylex.h
	$(CC) -o $(OBJ)kyanite.o -c $(SRC)kyanite.c $(CCFLAGS)

$(OBJ)kylex.o: $(SRC)kylex.c $(INC)kylex.h $(INC)kyanite.h
	$(CC) -o $(OBJ)kylex.o -c $(SRC)kylex.c $(CCFLAGS)

prepare:
	mkdir -p $(OBJ)

clean:
	rm -f $(OBJ)*.o
	rm -f $(OUT)kyanite