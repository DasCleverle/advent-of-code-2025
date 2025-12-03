CC = gcc
CFLAGS = -std=c2x -Wall -Wextra -Wconversion -Wpedantic -Werror -I./lib -lm
TARGET = target/

day%: day%.c lib/*.c
	mkdir -p $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET)$@ $< lib/*.c

clean:
	rm -r $(TARGET)
