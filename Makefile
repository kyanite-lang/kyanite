OUT=bin/
OBJ=$(OUT)obj/
SRC=src/
INC=$(SRC)includes/

CC=gcc -std=gnu99

CCFLAGS=-O2 -Wall -Wextra
LDFLAGS=

HEADERS=$(INC)kyanite.h $(INC)kyparse.h $(INC)kyobject.h

$(OUT)kyanite: $(OBJ)kyanite.o $(OBJ)kylex.o
	$(CC) -o $(OUT)kyanite $(OBJ)kyanite.o $(OBJ)kylex.o $(LDFLAGS)

$(OBJ)kyanite.o: $(SRC)kyanite.c $(HEADERS)
	$(CC) -o $(OBJ)kyanite.o -c $(SRC)kyanite.c $(CCFLAGS)

$(OBJ)kylex.o: $(SRC)kylex.c $(HEADERS)
	$(CC) -o $(OBJ)kylex.o -c $(SRC)kylex.c $(CCFLAGS)

prepare:
	mkdir -p $(OBJ)

clean:
	rm -f $(OBJ)*.o
	rm -f $(OUT)kyanite