CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assembler/assemble emulate

emulate: cpu.o memory_proc.o ALU.o
assembler/assemble: assembler/dictionary.o assembler/encode.o

clean:
	rm -f $(wildcard *.o)
	rm -f assembler/assemble
	rm -f emulate
