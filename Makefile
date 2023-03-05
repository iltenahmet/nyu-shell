CC=gcc
CFLAGS=-g -pedantic -std=gnu17 -Wall -Werror -Wextra

.PHONY: all
all: Nyush

Nyush: Nyush.o BuiltInCommands.o

Nyush.o: Nyush.c nyush.h BuiltInCommands.h

BuiltInCommands.o: BuiltInCommands.c BuiltInCommands.h

.PHONY: clean
clean:
	rm -f *.o Nyush

