CC = gcc
CFLAGS = -I.
DEPS = database.h

%.o: &.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

port: main.o database.o
	$(CC) -o port main.o database.o