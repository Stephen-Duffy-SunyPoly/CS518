#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <assets.h>
#include "qnd/frameRate.hpp"
#include "qnd/spriteSheet.hpp"
#include "qnd/initializer.hpp"


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
    Initializer init(SDL_INIT_VIDEO);

    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    SDL_Texture * background = nullptr;
    SDL_Texture * ball = nullptr;
    SDL_Texture * fontSmallTexture = nullptr;
    SDL_Texture * fontLargeTexture = nullptr;
    SDL_Texture * manSpriteSheetTexture = nullptr;

    init.addWindowSizeOfTexture("p6_duffysd",0, &window, &renderer,background_png_data, background_png_size, &background)
    .addTexture(ball_bmp_data, ball_bmp_size, &ball)
    .addTexture(font_11x16_png_data, font_11x16_png_size, &fontSmallTexture,true)
    .addTexture(font_22x32_png_data, font_22x32_png_size, &fontLargeTexture,true)
    .addTexture(mann_png_data, mann_png_size, &manSpriteSheetTexture);


    const int width = init.getWindowWidth();
    const int height = init.getWindowHeight();


    Font fontSmall(fontSmallTexture);
    Font fontLarge(fontLargeTexture);
    SpriteSheet manSprite(manSpriteSheetTexture,12,3);
    SDL_SetTextureScaleMode(manSpriteSheetTexture, SDL_SCALEMODE_NEAREST);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_FPoint playerPosition{300.0f,645.0f};
    SDL_FPoint ballPos{300.0f,200.0f};
    SDL_FPoint ballVelocity{0,3};
    int playerMovementAnimationFrame = 0;

    const float ground = playerPosition.y + static_cast<float>(manSprite.getHeight())*1.5f;

    int score=0;
    int highScore =0;

    bool should_quit = false;
    FrameRate frameRate;
    while (!should_quit) {
        SDL_Event event;
        bool mouseClicked = false;
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
                                ballVelocity = {static_cast<float>(SDL_rand(30) - 15)/2.f,-static_cast<float>(SDL_rand(170))/10.0f-12.0f};
                                score = 0;
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    mouseClicked = true;
                    break;
                default:
                    break;
            }
        }

        //draw the background
        SDL_RenderTexture(renderer, background,nullptr,nullptr);

        SDL_FPoint mouse{};
        SDL_GetMouseState(&mouse.x, &mouse.y);

        float playerMovementDistance = ((mouse.x-static_cast<float>(manSprite.getWidth())/2.0f) - playerPosition.x) *0.04f;
        float moveAmt = SDL_fabsf(playerMovementDistance);
        int animationSet = 0;
        if (moveAmt > 1.05) {
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
            if (score > highScore) {
                highScore = score;
            }
        }
        //if hit the walls, bounce and reduce speed
        if (ballPos.x <=0 || ballPos.x >= static_cast<float>(width) - 64) {
            ballVelocity.x = -ballVelocity.x * 0.7f;
            ballPos.x += ballVelocity.x;
        }

        //move the ball and apply acceleration
        ballPos += ballVelocity;
        ballVelocity.y += GRAVITY;
        ballVelocity.x *= AIR_DRAG;

        constexpr float hitAreaSize = 55;
        if (mouseClicked) {
            //get direction from the player
            SDL_FPoint ballCenter = ballPos + SDL_FPoint{32.0f,32.0f};
            SDL_FPoint playerHitPoint = playerPosition + SDL_FPoint{static_cast<float>(manSprite.getWidth())*1.5f/2.0f,40.0f};
            SDL_FPoint direction = ballCenter - playerHitPoint;
            float directionDistanceSquared = *direction;
            if (directionDistanceSquared < hitAreaSize*hitAreaSize) {
                float powerScale = -0.99f*(-directionDistanceSquared/(hitAreaSize*hitAreaSize) + 1.0f) + 1.3f;
                ballVelocity = direction * powerScale;
                ballVelocity *= 1.1f;
                ballVelocity.y *= 0.6f;
                //if it was hit while not on the ground, increase the score
                if (ballPos.y < ground-32) {
                    score++;
                }
            }
        }
        // SDL_FRect bbp{
        //     .x = static_cast<float>(manSprite.getWidth())*1.5f/2.0f-hitAreaSize + playerPosition.x,
        //     .y = 40.0f-hitAreaSize + playerPosition.y,
        //     .w = hitAreaSize*2,
        //     .h = hitAreaSize*2
        // };
        // SDL_SetRenderDrawColor(renderer,0,0,0,60);
        // SDL_RenderFillRect(renderer,&bbp);

        SDL_FRect bd{ballPos.x,ballPos.y,static_cast<float>(ball->w),static_cast<float>(ball->h)};
        SDL_RenderTextureRotated(renderer, ball, nullptr,&bd,ballPos.x,nullptr,SDL_FLIP_NONE);

        if (ballPos.y >= ground-32) {
            //when the ball is on the ground
            char scoreReport[100];
            SDL_snprintf(scoreReport,100,"You Scored: %d",score);
            fontLarge.drawStringCentered(renderer,scoreReport,width/2,height/2);
        }
        char scoreDisplay[100];
        char highScoreDisplay[100];
        SDL_snprintf(scoreDisplay,100,"Score: %d",score);
        SDL_snprintf(highScoreDisplay,100,"High Score: %d",highScore);
        fontSmall.drawString(renderer,scoreDisplay,10,10);
        fontSmall.drawString(renderer,highScoreDisplay,10,10+fontSmall.getHeight());

        SDL_RenderPresent(renderer);
        frameRate.delay();

    }





    //clean up zone
    //no longer necessary! This functionality is handled automatically but the destructor of the initializer
    return 0;
}