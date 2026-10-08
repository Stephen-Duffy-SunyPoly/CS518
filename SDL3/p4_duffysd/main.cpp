#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <assets.h>
#include "frameRate.hpp"


struct Pos {
    float x, y;
    Pos operator*(const Pos &other) const {
        //get 10% of the way there
        const float xDif = other.x - x ;
        const float yDif = other.y - y;
        return {.x = x+xDif*0.1f, .y = y+yDif*0.1f};
    }
};

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_IOStream * backgroundIoStream = SDL_IOFromConstMem(backdrop_jpeg_data, backdrop_jpeg_size);
    SDL_Surface * backgroundSurface = IMG_Load_IO(backgroundIoStream,true);

    SDL_Window * window;
    SDL_Renderer * renderer;

    if (!SDL_CreateWindowAndRenderer("p4_duffysd",backgroundSurface->w,backgroundSurface->h,0,&window, &renderer)) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        SDL_DestroySurface(backgroundSurface);
        return 1;
    }
    SDL_Texture * background = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_DestroySurface(backgroundSurface);

    SDL_IOStream * pizzaIoStream = SDL_IOFromConstMem(pizza_png_data, pizza_png_size);
    SDL_Surface * pizzaSurface = IMG_Load_IO(pizzaIoStream,true);
    SDL_SetSurfaceColorKey(pizzaSurface,true, *static_cast<Uint32 *>(pizzaSurface->pixels));
    SDL_Cursor * cursor = SDL_CreateColorCursor(pizzaSurface,pizzaSurface->w/2,1);
    SDL_SetCursor(cursor);
    SDL_DestroySurface(pizzaSurface);

    SDL_IOStream * puscheenIoStream = SDL_IOFromConstMem(pusheen_png_data, pusheen_png_size);
    SDL_Surface * puscheenSurface = IMG_Load_IO(puscheenIoStream,true);
    SDL_SetSurfaceColorKey(puscheenSurface, true, *static_cast<Uint32 *>(puscheenSurface->pixels));
    SDL_Texture * pucheen = SDL_CreateTextureFromSurface(renderer, puscheenSurface);
    SDL_DestroySurface(puscheenSurface);

    Pos positionBuffer[30] = {};
    int lastPos = 0;

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
        Pos mouse{};
        SDL_GetMouseState(&mouse.x, &mouse.y);

        //draw the background
        SDL_RenderTexture(renderer, background,nullptr,nullptr);

        //update the new position
        positionBuffer[(lastPos+1)%30] = positionBuffer[lastPos] * mouse;
        lastPos = (lastPos +1) %30;

        //render the pucheens
        for (int i=0;i<30;i++) {
            Pos pos = positionBuffer[(lastPos+i) % 30];
            SDL_FRect destPos = {.x = pos.x-25,.y = pos.y-20,.w = static_cast<float>(pucheen->w),.h = static_cast<float>(pucheen->h)};
            SDL_RenderTexture(renderer, pucheen,nullptr, &destPos);
        }

        SDL_RenderPresent(renderer);
        frameRate.delay();
    }





    //clean up zone
    SDL_DestroyCursor(cursor);
    SDL_DestroyTexture(pucheen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}