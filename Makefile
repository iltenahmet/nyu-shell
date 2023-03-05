CC=gcc
CFLAGS=-g -pedantic -std=gnu17 -Wall -Werror -Wextra

.PHONY: all
all: nyush

nyush: nyush.o builtInCommands.o

nyush.o: nyush.c nyush.h builtInCommands.h

builtInCommands.o: builtInCommands.c builtInCommands.h

.PHONY: clean
clean:
	rm -f *.o nyush

