CFLAGS = -Wall -g -c
CC = gcc

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

TARGET = table

$(TARGET):$(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)