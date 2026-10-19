#include "../include/Memory.h"
#include "../include/CPU.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <iomanip>

#define WIDTH 160
#define HEIGHT 120
#define FRAMEBUFFER 0xb1df
#define KEYBOARD 0xfce0

int main(int argc, char **argv){
    if (argc!=2){
        std::cout<<"Incorrect usage. ex emulator <filename>.bin"<<std::endl;
        return 1;
    }
    Memory memory;
    if (!memory.loadProgram(argv[1])) return 1;
    CPU cpu(memory);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window=SDL_CreateWindow("CPU", WIDTH*4, HEIGHT*4, SDL_WINDOW_OPENGL);
    SDL_Surface *windowSurface=SDL_GetWindowSurface(window);
    uint8_t *pixels=&memory.getMemory()[FRAMEBUFFER];
    SDL_Surface *surface=SDL_CreateSurfaceFrom(WIDTH, HEIGHT, SDL_PIXELFORMAT_RGB332, pixels, WIDTH);
    SDL_BlitSurfaceScaled(surface, NULL, windowSurface, NULL, SDL_SCALEMODE_NEAREST);
    SDL_UpdateWindowSurface(window);
    SDL_Event event;
    bool open=true;
    while (open){
        while (SDL_PollEvent(&event)){
            if (event.type==SDL_EVENT_QUIT){
                open=false;
            }
            if (event.key.down){
                if (event.key.key==SDLK_UP){
                    memory.writeByte(0x1, KEYBOARD);
                }else if (event.key.key==SDLK_DOWN){
                    memory.writeByte(0x2, KEYBOARD);
                }else if (event.key.key==SDLK_W){
                    memory.writeByte(0x3, KEYBOARD);
                }else if (event.key.key==SDLK_S){
                    memory.writeByte(0x4, KEYBOARD);
                }
            }else{
                memory.writeByte(0x00, KEYBOARD);
            }
        }
        cpu.fetch();
        cpu.decode();
        cpu.exec();
        SDL_BlitSurfaceScaled(surface, NULL, windowSurface, NULL, SDL_SCALEMODE_NEAREST);
        SDL_UpdateWindowSurface(window);
    }
    return 0;
}
