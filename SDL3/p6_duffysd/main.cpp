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

    SDL_Texture * ball = nullptr;
    {
        SDL_IOStream * memoryIoStream = SDL_IOFromConstMem(ball_bmp_data, ball_bmp_size);
        SDL_Surface * tmpImg = IMG_Load_IO(memoryIoStream,true);
        if (tmpImg == nullptr) {
            SDL_Log("Unable to load image %s", SDL_GetError());
            SDL_DestroyTexture(background);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        ball = SDL_CreateTextureFromSurface(renderer, tmpImg);
        SDL_DestroySurface(tmpImg);
    }
    if (ball == nullptr) {
        SDL_Log("Unable to create texture from %s", SDL_GetError());
        SDL_DestroyTexture(background);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture * fontSmallTexture = nullptr;
    {
        SDL_IOStream * memoryIoStream = SDL_IOFromConstMem(font_11x16_png_data, font_11x16_png_size);
        SDL_Surface * tmpImg = IMG_Load_IO(memoryIoStream,true);
        if (tmpImg == nullptr) {
            SDL_Log("Unable to load image %s", SDL_GetError());
            SDL_DestroyTexture(ball);
            SDL_DestroyTexture(background);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        SDL_SetSurfaceColorKey(tmpImg,true, *static_cast<Uint32 *>(tmpImg->pixels));
        fontSmallTexture = SDL_CreateTextureFromSurface(renderer, tmpImg);
        SDL_DestroySurface(tmpImg);
    }
    if (fontSmallTexture == nullptr) {
        SDL_Log("Unable to create texture from %s", SDL_GetError());
        SDL_DestroyTexture(ball);
        SDL_DestroyTexture(background);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    Font fontSmall(fontSmallTexture);

    SDL_Texture * fontLargeTexture = nullptr;
    {
        SDL_IOStream * memoryIoStream = SDL_IOFromConstMem(font_22x32_png_data, font_22x32_png_size);
        SDL_Surface * tmpImg = IMG_Load_IO(memoryIoStream,true);
        if (tmpImg == nullptr) {
            SDL_Log("Unable to load image %s", SDL_GetError());
            SDL_DestroyTexture(fontSmallTexture);
            SDL_DestroyTexture(ball);
            SDL_DestroyTexture(background);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        SDL_SetSurfaceColorKey(tmpImg,true, *static_cast<Uint32 *>(tmpImg->pixels));
        fontLargeTexture = SDL_CreateTextureFromSurface(renderer, tmpImg);
        SDL_DestroySurface(tmpImg);
    }
    if (fontLargeTexture == nullptr) {
        SDL_Log("Unable to create texture from %s", SDL_GetError());
        SDL_DestroyTexture(fontSmallTexture);
        SDL_DestroyTexture(ball);
        SDL_DestroyTexture(background);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    Font fontLarge(fontLargeTexture);

    SDL_Texture * manSpriteSheetTexture = nullptr;
    {
        SDL_IOStream * memoryIoStream = SDL_IOFromConstMem(mann_png_data, mann_png_size);
        SDL_Surface * tmpImg = IMG_Load_IO(memoryIoStream,true);
        if (tmpImg == nullptr) {
            SDL_Log("Unable to load image %s", SDL_GetError());
            SDL_DestroyTexture(fontLargeTexture);
            SDL_DestroyTexture(fontSmallTexture);
            SDL_DestroyTexture(ball);
            SDL_DestroyTexture(background);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        manSpriteSheetTexture = SDL_CreateTextureFromSurface(renderer, tmpImg);
        SDL_DestroySurface(tmpImg);
    }
    if (manSpriteSheetTexture == nullptr) {
        SDL_Log("Unable to create texture from %s", SDL_GetError());
        SDL_DestroyTexture(fontLargeTexture);
        SDL_DestroyTexture(fontSmallTexture);
        SDL_DestroyTexture(ball);
        SDL_DestroyTexture(background);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SpriteSheet manSprite(manSpriteSheetTexture,12,3);
    SDL_SetTextureScaleMode(manSpriteSheetTexture, SDL_SCALEMODE_NEAREST);


    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    int cnt = 0;

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

        fontLarge.drawString(renderer,"Large test", 10, 10);
        fontSmall.drawString(renderer,"Small test", 10, 50);

        manSprite.draw(renderer,cnt,150,150,1.5f);

        SDL_FRect bd{
            200,200,(float)ball->w,(float)ball->h
        };
        SDL_RenderTextureRotated(renderer, ball, nullptr,&bd,15,nullptr,SDL_FLIP_NONE);

        SDL_RenderPresent(renderer);
        frameRate.delay();
        cnt++;
        cnt %= 12*3;
    }





    //clean up zone

    SDL_DestroyTexture(manSpriteSheetTexture);
    SDL_DestroyTexture(fontLargeTexture);
    SDL_DestroyTexture(fontSmallTexture);
    SDL_DestroyTexture(ball);
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}