#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_gfxPrimitives.h>
#include "frameRate.hpp"

constexpr SDL_InitFlags initFLags = SDL_INIT_VIDEO;

static auto backgroundFile = "../backdrop.jpeg";
static auto mouseFile = "../pizza.png";

int main(int argc, char* argv[]) {

    if (!SDL_Init(initFLags)) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Surface * tmpImage = IMG_Load(backgroundFile);

    if (tmpImage == nullptr) {
        std::cerr << "Failed to load image: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    int width = tmpImage->w;
    int height = tmpImage->h;

    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("SDL Image Window",width,height,SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT, &window, &renderer)) {
        std::cerr << "SDL Window creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(tmpImage);
        SDL_Quit();
    }
    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(tmpImage);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Texture * background = SDL_CreateTextureFromSurface(renderer, tmpImage);
    SDL_DestroySurface(tmpImage);
    tmpImage = nullptr;
    tmpImage = IMG_Load(mouseFile);
    if (tmpImage == nullptr) {
        std::cerr << "Failed to load image: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }
    SDL_SetSurfaceColorKey(tmpImage, true, *static_cast<Uint32*>(tmpImage->pixels));
    SDL_Texture * mouseImage = SDL_CreateTextureFromSurface(renderer, tmpImage);
    SDL_DestroySurface(tmpImage);
    tmpImage = nullptr;


    SDL_RenderTexture(renderer, background, nullptr, nullptr);

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

        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        SDL_RenderTexture(renderer, background, nullptr, nullptr);
        SDL_FRect mouseRect{
            .x = mouseX - static_cast<float>(mouseImage->w)/2.0f,
            .y = mouseY - static_cast<float>(mouseImage->h)/2.0f,
            .w = static_cast<float>(mouseImage->w),
            .h = static_cast<float>(mouseImage->h)
        };
        SDL_RenderTexture(renderer, mouseImage, nullptr, &mouseRect);
        circleColor(renderer, mouseX, mouseY, 18, SDL_rand_bits());

        SDL_RenderPresent(renderer);
        fr.delay();
    }

    SDL_DestroyTexture(background);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}