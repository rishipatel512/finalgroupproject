CC = gcc
CFLAGS = -Wall -Wextra -g

myshell: main.o parse.o execute.o builtin.o signals.o logger.o
	$(CC) $(CFLAGS) main.o parse.o execute.o builtin.o signals.o logger.o -o myshell

main.o: main.c parse.h execute.h builtin.h signals.h logger.h
	$(CC) $(CFLAGS) -c main.c

parse.o: parse.c parse.h
	$(CC) $(CFLAGS) -c parse.c

execute.o: execute.c execute.h logger.h
	$(CC) $(CFLAGS) -c execute.c

builtin.o: builtin.c builtin.h
	$(CC) $(CFLAGS) -c builtin.c

signals.o: signals.c signals.h
	$(CC) $(CFLAGS) -c signals.c

logger.o: logger.c logger.h
	$(CC) $(CFLAGS) -c logger.c

%.o:%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o  myshell.log
