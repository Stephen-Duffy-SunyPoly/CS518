#include <iostream>
#include <SDL3/SDL.h>
// #include <SDL3/SDL_main.h>

constexpr SDL_InitFlags initFLags = SDL_INIT_VIDEO;

int main() {
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
            Uint8 c = static_cast<Uint8>(SDL_rand(256));
            //transparency does not work like i would expect, 0 alpha is not 100% transparent unless all other color channels are off
            static_cast<Uint32*>(surface->pixels)[x+y*width] = SDL_MapSurfaceRGBA(surface, c,c,c,static_cast<Uint8>(SDL_rand(256)));// | 0xFF000000;
        }
    }

    auto pfmd = SDL_GetPixelFormatDetails(surface->format);

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

        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        double t = (mouseX+mouseY) / static_cast<double>(width+height);

        //do other render stuff
        auto* pixels = static_cast<Uint32*>(surface->pixels);
        for (int y=0;y<height;y++) {
            for (int x =0;x<width;x++){
                int i = x+y*width;
                Uint8 c;
                Uint8 a;
                SDL_GetRGBA(pixels[i],pfmd, nullptr,&c, nullptr,nullptr,&a);

                auto c2 = static_cast<Uint8>(SDL_rand(256));
                c = static_cast<Uint8>(SDL_round(t * c + (1.0 - t) * c2));
                pixels[i] = SDL_MapSurfaceRGBA(surface, c,c,c,a);// | 0xFF000000;
            }
        }

        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}