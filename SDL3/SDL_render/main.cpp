#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "color.h"
#include "frameRate.hpp"

constexpr SDL_InitFlags initFLags = SDL_INIT_VIDEO;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Please provide an image!" << std::endl;
        return EXIT_FAILURE;
    }

    if (!SDL_Init(initFLags)) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Surface * image = IMG_Load(argv[1]);
    if (image == nullptr) {
        std::cerr << "Failed to load image: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    int width = image->w;
    int height = image->h;

    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("SDL Image Window",width,height,SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT, &window, &renderer)) {
        std::cerr << "SDL Window creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(image);
        SDL_Quit();
    }
    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(image);
        SDL_Quit();
        return EXIT_FAILURE;
    }


    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);



    SDL_RenderTexture(renderer, texture, nullptr, nullptr);

    int pixelsPerRow = static_cast<int>(image->pitch / sizeof(Uint32));
    auto * pixels = static_cast<Uint32 *>(image->pixels);
    const SDL_PixelFormatDetails * pixelFormatDetails = SDL_GetPixelFormatDetails(image->format);

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

        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        fr.delay();
    }

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(image);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}