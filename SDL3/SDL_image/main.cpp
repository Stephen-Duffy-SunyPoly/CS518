#include <iostream>
#include <ctime>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

constexpr SDL_InitFlags initFLags = SDL_INIT_VIDEO;

int main(int argc, char* argv[]) {
    uint32_t seed;
    seed = time(nullptr) ^ reinterpret_cast<intptr_t>(&seed) ^ reinterpret_cast<intptr_t>(&seed) >> 16;
    srand(seed);

    if (!SDL_Init(initFLags)) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Hello World!" << std::endl;

    int width = 1280;
    int height = 720;

    SDL_Window * window = SDL_CreateWindow("Basic SDL Window",width,height,SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT);
    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Surface * surface = SDL_GetWindowSurface(window);

    std::string pixelFormat = SDL_GetPixelFormatName(surface->format);
    if (pixelFormat != "SDL_PIXELFORMAT_ARGB8888") {
        std::cerr << "Alert! Non standard pixel format! " << pixelFormat << std::endl;
    }



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
                surface = SDL_GetWindowSurface(window);
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

        //do other render stuff


        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}