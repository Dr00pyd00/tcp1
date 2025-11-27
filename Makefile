
# MakeFile

CC = gcc
CFLAGS = -Wall -pedantic -g -Wextra

SOURCES = tcp.c server.c
OBJS =  $(SOURCES:.c=.o)
TARGET = server


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c tcp.h
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm *.o $(TARGET)
