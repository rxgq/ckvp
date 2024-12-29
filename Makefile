CC = gcc
CFLAGS = -Wall -g -std=c99

SRCS = src/main.c src/kvp_store.c src/tests.c
OBJS = $(SRCS:.c=.o)

TARGET = program

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c KvpStore.h
	$(CC) $(CFLAGS) -c $< -o $@
