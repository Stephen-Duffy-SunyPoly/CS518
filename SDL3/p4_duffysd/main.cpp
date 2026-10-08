#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_gfxPrimitives.h>
#include <assets.h>
#include "frameRate.hpp"
#include "color.h"


struct Pos {
    float x, y;
    Pos operator*(const Pos &other) const {
        //get 10% of the way there
        const float xDif = other.x - x;
        const float yDif = other.y - y;
        return {.x = x+xDif*0.1f, .y = y+yDif*0.1f};
    }

    float operator^(const Pos &other) const {
        const float xDif = other.x - x;
        const float yDif = other.y - y;
        return xDif*xDif + yDif*yDif;
    }
};

struct MyColor {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
    Uint32 operator*() const {
        return a << 24 | b << 16 | g << 8 | r;
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

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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

    SDL_SetTextureBlendMode(pucheen,SDL_BLENDMODE_BLEND);

    Pos positionBuffer[30] = {};
    int lastPos = 0;
    bool radiance[360] = {};
    int radianceRoll = 0;
    MyColor colorLut[360] = {};
    for (int i = 0; i < 360; ++i) {
        MyColor color{};
        convertHSVtoRGB(static_cast<float>(i),1,1,&color.r,&color.g,&color.b);
        color.a = 255;
        colorLut[i] = color;
    }

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
        //moving every frame is fine, I think your frame rate limiter is just broken
        positionBuffer[(lastPos+1)%30] = positionBuffer[lastPos] * mouse;
        lastPos = (lastPos +1) %30;

        float distanceTotal = 0;
        //render the pucheens
        for (int i=0;i<30;i++) {
            Pos pos = positionBuffer[(lastPos+i) % 30];
            distanceTotal += pos ^ positionBuffer[(lastPos+i+1)%30];
            SDL_FRect destPos = {.x = pos.x-25,.y = pos.y-20,.w = static_cast<float>(pucheen->w),.h = static_cast<float>(pucheen->h)};
            SDL_SetTextureAlphaModFloat(pucheen, (static_cast<float>(i)+1.0f)/30.0f);
            SDL_RenderTexture(renderer, pucheen,nullptr, &destPos);
        }

        if (distanceTotal < 1) {
            //so the docs say the color is RGBA and the code seems to think that, however it ia actually ABGR
            for (int i=0;i<180;i++) {
                if (radiance[i]) {
                    MyColor color = colorLut[(i+radianceRoll)%360];
                    circleColor(renderer,static_cast<Sint16>(mouse.x), static_cast<Sint16>(mouse.y),30+i,*color);
                }
            }
            radiance[radianceRoll%360] = true;
            radianceRoll++;
            radianceRoll %= 360;
            // circleColor(renderer, static_cast<Sint16>(mouse.x), static_cast<Sint16>(mouse.y), 30,0xFF00FF00);
        } else {
            //reset it
            SDL_memset(radiance,0,sizeof(radiance));
            radianceRoll = 0;
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