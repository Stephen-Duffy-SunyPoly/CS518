#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

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
    const int imgSize = width * height;

    SDL_Window * window = SDL_CreateWindow("SDL Image Window",width,height,SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT);
    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(image);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Surface * surface = SDL_GetWindowSurface(window);
    SDL_Surface *image_c = SDL_ConvertSurface(image, surface->format);
    SDL_DestroySurface(image);
    image = image_c;
    auto * imagePixels = static_cast<Uint32*>(image->pixels);

    //blit stands for BLock Transfer
    SDL_BlitSurface(image, nullptr, surface, nullptr);

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

        SDL_BlitSurface(image, nullptr, surface, nullptr);
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroySurface(image);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}