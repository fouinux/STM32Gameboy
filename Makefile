CC = gcc

CFLAGS = -g
LDFLAGS = -lSDL2

SRCS := $(wildcard main.c gameboy/*.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

TARGET = STM32Gameboy

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS)

.PHONY: all clean