CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

myshell: src/main.o src/parse.o src/execute.o src/builtin.o src/signals.o src/logger.o
	$(CC) $(CFLAGS) -o myshell src/main.o src/parse.o src/execute.o src/builtin.o src/signals.o src/logger.o

src/main.o: src/main.c include/parse.h include/execute.h include/builtin.h include/signals.h include/logger.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/parse.o: src/parse.c include/parse.h
	$(CC) $(CFLAGS) -c src/parse.c -o src/parse.o

src/execute.o: src/execute.c include/execute.h include/logger.h
	$(CC) $(CFLAGS) -c src/execute.c -o src/execute.o

src/builtin.o: src/builtin.c include/builtin.h
	$(CC) $(CFLAGS) -c src/builtin.c -o src/builtin.o

src/signals.o: src/signals.c include/signals.h
	$(CC) $(CFLAGS) -c src/signals.c -o src/signals.o

src/logger.o: src/logger.c include/logger.h
	$(CC) $(CFLAGS) -c src/logger.c -o src/logger.o

clean:
	rm -f src/*.o myshell
