CFLAGS = -Wall -g -lm
CC     = gcc $(CFLAGS)

PROGRAMS= \
					commando\
all : $(PROGRAMS)

commando : commando.o cmd.o cmdcol.o util.o
	$(CC) -o commando commando.o cmd.o cmdcol.o util.o

util.o : util.c commando.h
	$(CC) -c util.c

cmdcol.o : cmdcol.c commando.h
	$(CC) -c cmdcol.c

cmd.o : cmd.c commando.h
	$(CC) -c cmd.c

commando.o : commando.c commando.h
	$(CC) -c commando.c
#
# clean:
clean: 
	rm commando *.o
#Testing Targets
VALGRIND= valgrind --leak-check=full --show-leak-kinds=all

include test_Makefile
