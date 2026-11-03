#include <iostream>
#include <SDL3/SDL.h>
#include "qnd/frameRate.hpp"
#include "qnd/initializer.hpp"
#include <assets.h>

constexpr SDL_InitFlags initFLags = SDL_INIT_VIDEO;

int main(int argc, char* argv[]) {


    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    SDL_Texture * background = nullptr;

    qnd::Initializer init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    init.addWindowSizeOfTexture("SDL Audio Demo", SDL_WINDOW_RESIZABLE|SDL_WINDOW_TRANSPARENT,
       &window ,&renderer, shapes_png_data, shapes_png_size, &background);



    SDL_RenderTexture(renderer, background, nullptr, nullptr);

    qnd::FrameRate fr(30);

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


        SDL_RenderPresent(renderer);
        fr.delay();
    }

    return EXIT_SUCCESS;
}