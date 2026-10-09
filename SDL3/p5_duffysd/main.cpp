#include <SDL3/SDL.h>
#include <SDL3_gfxPrimitives.h>

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window * window;
    SDL_Renderer * renderer;

    if (!SDL_CreateWindowAndRenderer("p5_duffysd",1000,500,0,&window, &renderer)) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return 1;
    }

    bool should_quit = false;
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

        //logo 1
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_FRect rect{0.0f,0.0f,500.0f,500.0f};
        SDL_RenderRect(renderer,&rect);

        //logo 2
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        rect.x=500.0f;
        SDL_RenderRect(renderer,&rect);

        SDL_RenderPresent(renderer);
    }

    //clean up zone
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}