# Makefile for myshell

CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
TARGET = myshell

# Source files
SRCDIR = src
INCDIR = include
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/parse.c $(SRCDIR)/execute.c $(SRCDIR)/builtin.c $(SRCDIR)/signals.c $(SRCDIR)/logger.c

# Object files
OBJECTS = $(SRCDIR)/main.o $(SRCDIR)/parse.o $(SRCDIR)/execute.o $(SRCDIR)/builtin.o $(SRCDIR)/signals.o $(SRCDIR)/logger.o

# Header files
HEADERS = $(INCDIR)/parse.h $(INCDIR)/execute.h $(INCDIR)/builtin.h $(INCDIR)/signals.h $(INCDIR)/logger.h

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files
$(SRCDIR)/main.o: $(SRCDIR)/main.c $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRCDIR)/main.c -o $(SRCDIR)/main.o

$(SRCDIR)/parse.o: $(SRCDIR)/parse.c $(INCDIR)/parse.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/parse.c -o $(SRCDIR)/parse.o

$(SRCDIR)/execute.o: $(SRCDIR)/execute.c $(INCDIR)/execute.h $(INCDIR)/logger.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/execute.c -o $(SRCDIR)/execute.o

$(SRCDIR)/builtin.o: $(SRCDIR)/builtin.c $(INCDIR)/builtin.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/builtin.c -o $(SRCDIR)/builtin.o

$(SRCDIR)/signals.o: $(SRCDIR)/signals.c $(INCDIR)/signals.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/signals.c -o $(SRCDIR)/signals.o

$(SRCDIR)/logger.o: $(SRCDIR)/logger.c $(INCDIR)/logger.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/logger.c -o $(SRCDIR)/logger.o

# Clean up
clean:
	rm -f $(SRCDIR)/*.o $(TARGET) myshell.log

.PHONY: all clean
