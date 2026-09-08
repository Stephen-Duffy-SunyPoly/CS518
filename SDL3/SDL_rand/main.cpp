#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

constexpr SDL_InitFlags initFLags = SDL_INIT_VIDEO;

int main(int argc, char* argv[]) {
    Uint64 seed;
    seed = reinterpret_cast<Uint64>(&seed);

    SDL_srand(seed);

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

    for (int y=0;y<height;y++) {
        for (int x =0;x<width;x++){
            //transparency does not work like i would expect, 0 alpha is not 100% transparent unless all other color channels are off
            static_cast<Uint32*>(surface->pixels)[x+y*width] = SDL_rand_bits();// | 0xFF000000;
        }
    }


    bool shouldRun = true;
    while (shouldRun) {
        SDL_Event event;
        //read all events
        while (SDL_PollEvent(&event)) {

            //handle the event
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED || event.type == SDL_EVENT_QUIT) {
                shouldRun = false;
            } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                const int newWidth = event.window.data1;
                const int newHeight = event.window.data2;
                std::cout << "Resized: " << newWidth << "x" << newHeight << std::endl;
                width = newWidth;
                height = newHeight;
                surface = SDL_GetWindowSurface(window);

            }
        }

        //do other render stuff

        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}