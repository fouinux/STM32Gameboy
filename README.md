# STM32Gameboy

## Setup WSL2

Install [GWSL](https://opticos.github.io/gwsl/).

Install lib SDL2:
```
sudo apt install libsdl2-2.0-0 libsdl2-dev
```

## Build

```
gcc main.c gameboy/*.c -lSDL2 -o STM32Gameboy
```