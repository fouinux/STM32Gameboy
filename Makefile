CC = gcc

CFLAGS = -Wall
LDFLAGS = -lSDL2

SRCS := $(wildcard *.c gameboy/*.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

TARGET = STM32Gameboy

all: $(TARGET)

release: CFLAGS += -Werror
release: all

debug: CFLAGS += -g -DDEBUG -DPPU_DEBUG
debug: all

$(TARGET): $(OBJS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS)

.PHONY: all clean
