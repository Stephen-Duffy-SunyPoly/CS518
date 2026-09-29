#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "frameRate.hpp"


int main(int argc, char* argv[]) {
    constexpr SDL_InitFlags INIT_FLAGS = SDL_INIT_VIDEO;

    if (argc < 2) {
        SDL_Log("Usage: %s <Image File>", argv[0]);
        return 1;
    }

    //SDL initialization things
    if (!SDL_Init(INIT_FLAGS)) {
        SDL_Log("SDL could not initialize! SDL Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Surface * image;
    ((image = SDL_LoadBMP(argv[1]))) || ((image = SDL_LoadPNG(argv[1])));
    if (image == nullptr) {
        SDL_Log("Error loading Image! SDL Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Prog 2: duffysd",image->w,image->h,0);

    if (window == nullptr) {
        SDL_Log("Window could not be created! SDL Error: %s", SDL_GetError());
        SDL_DestroySurface(image);
        SDL_Quit();
        return 1;
    }

    SDL_Surface * surface = SDL_GetWindowSurface(window);
    if (surface == nullptr) {
        SDL_Log("Error obtaining window surface! SDL Error: %s", SDL_GetError());
        SDL_DestroySurface(image);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Surface * fixedImage = SDL_ConvertSurface(image,surface->format);
    SDL_DestroySurface(image);
    image = fixedImage;




    SDL_BlitSurface(image,nullptr,surface,nullptr);

    SDL_Event e;
    bool quit = false;

    FrameRate fr(60);
    //render loop
    while(!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT:
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    quit = true;
                    break;
                default:
                    break;
            }
        }


        SDL_UpdateWindowSurface(window);
        fr.delay();
    }

    SDL_DestroyWindow(window);
    SDL_DestroySurface(image);
    SDL_Quit();
    return 0;
}
