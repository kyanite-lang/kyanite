OUT=bin/
OBJ=$(OUT)obj/
SRC=src/
INC=$(SRC)includes/

CC=gcc -std=gnu99

CCFLAGS=-O2 -Wall -Wextra
LDFLAGS=

HEADERS=$(wildcard $(INC)*.h)
SRCS=$(wildcard $(SRC)*.c)
OBJS=$(SRCS:$(SRC)%.c=$(OBJ)%.o)

FIN=$(OUT)kyanite

all: $(FIN)

$(FIN): $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^

$(OBJ)%.o: $(SRC)%.c
	$(CC) -o $@ $(CCFLAGS) -c $<

.PHONY: prepare clean

prepare:
	mkdir -p $(OBJ)

clean:
	rm -f $(OBJ)*.o $(OUT)kyanite