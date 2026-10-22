#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <assets.h>
#include "frameRate.hpp"
#include "spriteSheet.hpp"


int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_IOStream * backgroundIoStream = SDL_IOFromConstMem(background_png_data, background_png_size);
    SDL_Surface * backgroundSurface = IMG_Load_IO(backgroundIoStream,true);

    SDL_Window * window;
    SDL_Renderer * renderer;

    if (!SDL_CreateWindowAndRenderer("p6_duffysd",backgroundSurface->w,backgroundSurface->h,0,&window, &renderer)) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        SDL_DestroySurface(backgroundSurface);
        return 1;
    }
    SDL_Texture * background = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_DestroySurface(backgroundSurface);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    bool should_quit = false;
    FrameRate frameRate;
    while (!should_quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    should_quit = true;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch (event.key.key) {
                        case SDLK_ESCAPE:
                            should_quit = true;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        // Pos mouse{};
        // SDL_GetMouseState(&mouse.x, &mouse.y);

        //draw the background
        SDL_RenderTexture(renderer, background,nullptr,nullptr);


        SDL_RenderPresent(renderer);
        frameRate.delay();
    }





    //clean up zone

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}