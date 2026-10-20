#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_gfxPrimitives.h>
#include "frameRate.hpp"
#include "spriteSheet.hpp"

constexpr SDL_InitFlags initFLags = SDL_INIT_VIDEO;

static const char * myString = "Hi, I am Jenny the";

int main(int argc, char* argv[]) {

    if (!SDL_Init(initFLags)) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    int width = 500;
    int height = 500;

    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("SDL Image Window",width,height,SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT, &window, &renderer)) {
        std::cerr << "SDL Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }
    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Texture * fontTexture = nullptr;
    {
        SDL_Surface * tmpImg = IMG_Load("../font_22x32.png");
        if (tmpImg == nullptr) {
            std::cerr << "IMG_Load failed: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            return EXIT_FAILURE;
        }
        SDL_SetSurfaceColorKey(tmpImg, true, *static_cast<Uint32*>(tmpImg->pixels));
        fontTexture = SDL_CreateTextureFromSurface(renderer, tmpImg);
        SDL_DestroySurface(tmpImg);
    }
    if (fontTexture == nullptr) {
        std::cerr << "font texture creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    Font font(fontTexture);

    FrameRate fr(30);

    bool shouldRun = true;
    while (shouldRun) {
        SDL_Event event;
        //read all events
        while (SDL_PollEvent(&event)) {
            //handle the event
            int newWidth;
            int newHeight;
            switch (event.type) {
            case SDL_EVENT_QUIT:
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                shouldRun = false;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                newWidth = event.window.data1;
                newHeight = event.window.data2;
                std::cout << "Resized: " << newWidth << "x" << newHeight << std::endl;
                width = newWidth;
                height = newHeight;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE) {
                    shouldRun = false;
                }
                break;
            default:
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 130, 40, 150, 128);
        SDL_RenderClear(renderer);


       font.drawStringCentered(renderer, myString, 250,250);


        SDL_RenderPresent(renderer);
        fr.delay();
    }

    SDL_DestroyTexture(fontTexture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}