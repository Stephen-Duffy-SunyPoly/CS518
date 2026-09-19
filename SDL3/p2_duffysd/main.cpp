#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


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

    // my program initialization things

    SDL_Event e;
    //render loop
    while(SDL_PollEvent(&e) == false || e.type != SDL_EVENT_QUIT) {
        SDL_UpdateWindowSurface(window);
    }

    //clean up
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
