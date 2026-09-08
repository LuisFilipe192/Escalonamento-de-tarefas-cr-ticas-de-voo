CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = scheduler
SRC = src/scheduler.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)