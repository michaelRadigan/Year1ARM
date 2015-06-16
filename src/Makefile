CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

emulate: emulator/cpu.o emulator/memory_proc.o emulator/ALU.o emulator/stack.o

assemble: assembler/dictionary.o assembler/encode.o assembler/genericstack.o


clean:
	rm -f $(wildcard *.o)
	rm -f assembler/assemble
	rm -f emulate
	rm -f emulator/cpu.o
	rm -f emulator/memory_proc.o
	rm -f emulator/ALU.o
	rm -f emulator/stack.o
