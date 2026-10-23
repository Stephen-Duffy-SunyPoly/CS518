#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <assets.h>
#include "frameRate.hpp"
#include "spriteSheet.hpp"


static SDL_FPoint operator+(const SDL_FPoint & a, const SDL_FPoint & b) {
    SDL_FPoint result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

static SDL_FPoint &operator+=(SDL_FPoint & a, const SDL_FPoint & b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

static SDL_FPoint operator-(const SDL_FPoint & a, const SDL_FPoint & b) {
    SDL_FPoint result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

static SDL_FPoint &operator*=(SDL_FPoint & a, const float b) {
    a.x *= b;
    a.y *= b;
    return a;
}

static SDL_FPoint operator*(const SDL_FPoint & a, const float b) {
    SDL_FPoint result;
    result.x = a.x * b;
    result.y = a.y * b;
    return result;
}

static float operator*(const SDL_FPoint &a) {
    return a.x*a.x + a.y*a.y;
}

constexpr int PLAYER_ANIMATION_SET_STANDING = 0;
constexpr int PLAYER_ANIMATION_SET_RUN_LEFT = 12;
constexpr int PLAYER_ANIMATION_SET_RUN_RIGHT = 24;
constexpr float GRAVITY = 0.75f;
constexpr float AIR_DRAG = 0.95f;

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_IOStream * backgroundIoStream = SDL_IOFromConstMem(background_png_data, background_png_size);
    SDL_Surface * backgroundSurface = IMG_Load_IO(backgroundIoStream,true);

    SDL_Window * window;
    SDL_Renderer * renderer;
    const int width = backgroundSurface->w;
    const int height = backgroundSurface->h;

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

    SDL_FPoint playerPosition{300.0f,645.0f};
    SDL_FPoint ballPos{300.0f,200.0f};
    SDL_FPoint ballVelocity{0,3};
    int playerMovementAnimationFrame = 0;

    const float ground = playerPosition.y + static_cast<float>(manSprite.getHeight())*1.5f;

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
                        case SDLK_SPACE:
                            if (ballPos.y+32 >= ground) {
                                ballVelocity = {static_cast<float>(SDL_rand(30) - 15)/10.0f,-static_cast<float>(SDL_rand(270))/10.0f-12.0f};
                            }
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

        SDL_FPoint mouse{};
        Uint32 mouseButtons = SDL_GetMouseState(&mouse.x, &mouse.y);

        float playerMovementDistance = ((mouse.x-static_cast<float>(manSprite.getWidth())/2.0f) - playerPosition.x) *0.04f;
        float moveAmt = SDL_fabsf(playerMovementDistance);
        int animationSet = 0;
        if (moveAmt > 1.1) {
            int direction = (playerMovementDistance > 0) ? 1:-1;
            if (moveAmt > 3.8) {
                //running speed
                if (direction == 1) {
                    animationSet = PLAYER_ANIMATION_SET_RUN_RIGHT;
                } else {
                    animationSet = PLAYER_ANIMATION_SET_RUN_LEFT;
                }
                playerMovementAnimationFrame += direction;
            } else {
                animationSet = PLAYER_ANIMATION_SET_STANDING;
                //standing but animating speed
                if (frameRate.getFrameCount()%2 == 0) {
                    playerMovementAnimationFrame += direction;
                }
            }

            playerMovementAnimationFrame %= 12;
            if (playerMovementAnimationFrame < 0) {
                playerMovementAnimationFrame += 12;
            }
        }else {
            animationSet = PLAYER_ANIMATION_SET_STANDING;
            playerMovementAnimationFrame = 0;
        }
        playerPosition.x += playerMovementDistance;
        manSprite.draw(renderer,animationSet + playerMovementAnimationFrame,playerPosition,1.5f);

        //ball physics
        //if it hit the ground, remove its speed
        if (ballPos.y > ground-32 && ballVelocity.y > 0) {
            ballVelocity.y = 0;
            ballVelocity.x *= AIR_DRAG;
        }
        if (ballPos.x <=0 || ballPos.x >= static_cast<float>(width) - 64) {
            ballVelocity.x = -ballVelocity.x * 0.7f;
            ballPos.x += ballVelocity.x;
        }

        ballPos += ballVelocity;
        ballVelocity.y += GRAVITY;
        ballVelocity.x *= AIR_DRAG;

        constexpr float hitAreaSize = 50;
        if (mouseButtons) {
            //get direction from the player
            SDL_FPoint ballCenter = ballPos + SDL_FPoint{32.0f,32.0f};
            SDL_FPoint playerHitPoint = playerPosition + SDL_FPoint{static_cast<float>(manSprite.getWidth())*1.5f/2.0f,40.0f};
            SDL_FPoint direction = ballCenter - playerHitPoint;
            float directionDistanceSquared = *direction;
            if (directionDistanceSquared < hitAreaSize*hitAreaSize) {
                float powerScale = -0.99f*(-directionDistanceSquared/(hitAreaSize*hitAreaSize) + 1.0f) + 1.3f;
                ballVelocity = direction * powerScale;
                ballVelocity.y *= 0.6f;
            }
        }
        SDL_FRect bbp{
            .x = static_cast<float>(manSprite.getWidth())*1.5f/2.0f-hitAreaSize + playerPosition.x,
            .y = 40.0f-hitAreaSize + playerPosition.y,
            .w = hitAreaSize*2,
            .h = hitAreaSize*2
        };
        SDL_SetRenderDrawColor(renderer,0,0,0,60);
        SDL_RenderFillRect(renderer,&bbp);

        SDL_FRect bd{ballPos.x,ballPos.y,static_cast<float>(ball->w),static_cast<float>(ball->h)};
        SDL_RenderTextureRotated(renderer, ball, nullptr,&bd,ballPos.x,nullptr,SDL_FLIP_NONE);

        SDL_RenderPresent(renderer);
        frameRate.delay();

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