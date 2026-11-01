CC = gcc

CFLAGS = -Wall -Wextra -I include

SOURCE = src/frame_parser.c app/main.c

TARGET = frame_parser.exe

all:
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

clean:
	del /Q $(TARGET) 2>NUL