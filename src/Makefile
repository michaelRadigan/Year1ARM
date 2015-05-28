CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o encode.o dictionary.o

emulate: emulate.o cpu.o memory_proc.o main.o

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
