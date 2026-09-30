// ReSharper disable CppUseInternalLinkage
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include "frameRate.hpp"
#include "color.h"


struct HSVColor {
    float h, s, v;
};

struct RGBColor {
    Uint8 r, g, b;
    HSVColor operator~() const {
        HSVColor hsv{};
        convertRGBtoHSV(r,g,b,&hsv.h,&hsv.s,&hsv.v);
        return hsv;
    }

    Uint32 operator<<(SDL_Surface * surface) const {
        return SDL_MapSurfaceRGB(surface,r,g,b);
    }
};

static RGBColor operator*(const HSVColor &hc) {
    RGBColor c{};
    convertHSVtoRGB(hc.h,hc.s,hc.v,&c.r,&c.g,&c.b);
    return c;
}

RGBColor getColor(Uint32 pd, const SDL_PixelFormatDetails *format) {
    RGBColor c{};
    SDL_GetRGB(pd, format, nullptr, &c.r, &c.g, &c.b);
    return c;
}

struct AreaBlock {
    int blockX;
    int blockY;
    int blockW;
    int blockH;
    float brightness;
    int animationProgress;
    int impactX;
    int impactY;
};

template<typename T>
void swap(T &a, T &b) noexcept {
    T tmp = a;
    a = b;
    b = tmp;
}

static int imap(const SDL_Surface * s, const int x, const int y) {
    int pixelsPerRow = static_cast<int>(s->pitch / sizeof(Uint32));
    return x+y*pixelsPerRow;
}

float valueOscillate(int animationFrame) {
    float frameF = static_cast<float>(animationFrame) / 16.0f;
    return SDL_sinf(2.5f*frameF) * SDL_expf(-0.4f*frameF) * 0.5f + 0.5f;
}


void desatBlockBlit(SDL_Surface * surface, HSVColor * hsvSurface, AreaBlock &block) {
    auto * pixels = static_cast<Uint32 *>(surface->pixels);
    int adjustedX = block.blockX * block.blockW;
    int adjustedY = block.blockY * block.blockH;
    for (int y=adjustedY; y < adjustedY + block.blockH && y < surface->h; y++) {
        for (int x=adjustedX; x < adjustedX + block.blockW && x < surface->w; x++) {
            //make a copy of this color
            HSVColor color = hsvSurface[y*surface->w + x];
            //modify its saturation
            color.s = 0.1;
            color.v *= block.brightness;
            //convert it to an RGB color and then convert it to a Uint32 for the surface
            pixels[imap(surface, x, y)] = *color << surface;
        }
    }
}

void dustNearImpact(HSVColor * hsvSurface, AreaBlock &block, int sWidth, int sHeight) {
    constexpr int NUMBER_OF_SWAPS = 160;
    //generate swap locations
    int indices[NUMBER_OF_SWAPS];
    //generate some random locations around the impact site
    for (int i=0;i<NUMBER_OF_SWAPS;i++) {
        int RNG = SDL_rand(0xFFF);
        int xoff = (RNG & 0x3F) - 31;
        int yoff = (RNG >> 6 & 0x3F) - 31;
        indices[i] = (yoff+block.impactY)*sWidth + (xoff+block.impactX);
    }
    int direction[] = {-sWidth,-sWidth+1,1,sWidth+1,sWidth,sWidth-1,-1,-sWidth-1};
    int max = sWidth * sHeight;
    for (int i=0;i<NUMBER_OF_SWAPS;i++) {
        //randomly decide where to swap them with
        int other = indices[i] + direction[SDL_rand((8))];
        if (indices[i] < max && other < max && indices[i] >= 0 && other >= 0) {
            swap(hsvSurface[indices[i]], hsvSurface[other]);
        }
    }
}

int main(int argc, char* argv[]) {
    constexpr SDL_InitFlags INIT_FLAGS = SDL_INIT_VIDEO;

    if (argc < 3) {
        SDL_Log("Usage: %s <Image File> <Image File>", argv[0]);
        return 1;
    }

    //SDL initialization things
    if (!SDL_Init(INIT_FLAGS)) {
        SDL_Log("SDL could not initialize! SDL Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Surface * image1 = IMG_Load(argv[1]);
    if (image1 == nullptr) {
        SDL_Log("Error loading Image! SDL Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Surface * image2 = IMG_Load(argv[2]);
    if (image2 == nullptr) {
        SDL_Log("Error loading Image! SDL Error: %s", SDL_GetError());
        SDL_DestroySurface(image1);
        SDL_Quit();
    }

    SDL_Window* window = SDL_CreateWindow("Prog 2: duffysd",image1->w,image1->h,0);

    if (window == nullptr) {
        SDL_Log("Window could not be created! SDL Error: %s", SDL_GetError());
        SDL_DestroySurface(image1);
        SDL_DestroySurface(image2);
        SDL_Quit();
        return 1;
    }

    SDL_Surface * surface = SDL_GetWindowSurface(window);
    if (surface == nullptr) {
        SDL_Log("Error obtaining window surface! SDL Error: %s", SDL_GetError());
        SDL_DestroySurface(image1);
        SDL_DestroySurface(image2);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    //reformat both images
    {
        SDL_Surface * fixedImage = SDL_ConvertSurface(image1,surface->format);
        SDL_DestroySurface(image1);
        image1 = fixedImage;
    }
    {
        SDL_Surface * fixedImage = SDL_ConvertSurface(image2,surface->format);
        SDL_DestroySurface(image2);
        image2 = fixedImage;
    }

    //resize img 2 to be the same size as img 1
    {
        SDL_Surface * tmpSurface = SDL_CreateSurface(image1->w,image1->h,surface->format);
        SDL_StretchSurface(image2,nullptr,tmpSurface,nullptr,SDL_SCALEMODE_LINEAR);
        SDL_DestroySurface(image2);
        image2 = tmpSurface;
    }

    //create a soperate buffer for the HSV version of img 2
    auto * image2HSV = new HSVColor[image2->w*image2->h];
    auto * image2Pixels = static_cast<Uint32 *>(image2->pixels);
    const SDL_PixelFormatDetails * image2FormatDetails = SDL_GetPixelFormatDetails(image2->format);

    for (int y=0;y<image2->h;y++) {
        for (int x=0;x<image2->w;x++) {
            image2HSV[y*image2->w+x] = ~getColor(image2Pixels[imap(image2,x,y)],image2FormatDetails);
        }
    }

    //create the blocks
    int numberOfBlocksHorizontal = static_cast<int>(SDL_ceil(image1->w / 200.0));
    int numberOfBlocksVertical = static_cast<int>(SDL_ceil(image1->h / 140.0));;
    int totalNumberOfBlocks = numberOfBlocksHorizontal*numberOfBlocksVertical;
    auto * blocks = new AreaBlock[totalNumberOfBlocks];
    for (int y=0;y<numberOfBlocksVertical;y++) {
        for (int x=0;x<numberOfBlocksHorizontal;x++) {
            blocks[y*numberOfBlocksHorizontal+x] = {
                x, y, 200, 180, 0.5f, 0
            };
        }
    }

    SDL_Surface * workingSurface = SDL_CreateSurface(surface->w,surface->h,surface->format);


    SDL_BlitSurface(image1,nullptr,workingSurface,nullptr);
    SDL_BlitSurface(workingSurface,nullptr,surface,nullptr);

    SDL_Surface * cannonBallSource = SDL_CreateSurface(60,60,SDL_PIXELFORMAT_RGBA8888);
    {
        auto * cannonPixels = static_cast<Uint32 *>(cannonBallSource->pixels);
        //draw a circle on that surface
        for (int y=0;y<cannonBallSource->h;y++) {
            int yd = cannonBallSource->h/2 - y;
            for (int x=0;x<cannonBallSource->w;x++) {
                int xd = cannonBallSource->w/2 - x;
                if (SDL_sqrtf(static_cast<float>(yd * yd + xd * xd)) < 30) {
                    cannonPixels[imap(cannonBallSource,x,y)] = SDL_MapSurfaceRGB(cannonBallSource,0,0,0);
                }
            }
        }
    }

    float cannonBallAngle = 0;
    bool cannonShooting = false;
    float cannonBallX = 0, cannonBallY = 0;
    float targetX = 0, targetY = 0;
    int targetBlockX = 0, targetBlockY = 0;

    constexpr float cannonBallSpeed = 15.f;

    SDL_Event e;
    bool quit = false;

    FrameRate fr(60);

    float mouseX, mouseY;
    //render loop
    while(!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT:
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    quit = true;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch (e.key.key) {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    switch (e.button.button) {
                        case SDL_BUTTON_LEFT: {
                            if (!cannonShooting) {
                                targetBlockX = static_cast<int>(e.button.x) / blocks[0].blockW;
                                targetBlockY = static_cast<int>(e.button.y) / blocks[0].blockH;
                                targetX = e.button.x;
                                targetY = e.button.y;
                                cannonBallX = static_cast<float>(surface->w)/2.f;
                                cannonBallY = static_cast<float>(surface->h);
                                cannonBallAngle = SDL_atan2f(targetY - static_cast<float>(surface->h),targetX - static_cast<float>(surface->w)/2.0f);
                                cannonShooting = true;
                            }
                        }
                            break;

                        default:
                            break;
                    }

                default:
                    break;
            }
        }
        SDL_GetMouseState(&mouseX,&mouseY);

        for (int i=0;i<totalNumberOfBlocks;i++) {
            if (blocks[i].animationProgress > 0 && blocks[i].animationProgress < 160) {
                blocks[i].brightness = valueOscillate(blocks[i].animationProgress);
                if (blocks[i].animationProgress < 50) {
                    dustNearImpact(image2HSV, blocks[i], workingSurface->w, workingSurface->h);
                }
                desatBlockBlit(workingSurface,image2HSV,blocks[i]);
                blocks[i].animationProgress++;
            }
        }

        SDL_BlitSurface(workingSurface,nullptr,surface,nullptr);

        if (cannonShooting) {
            cannonBallX += SDL_cosf(cannonBallAngle) * cannonBallSpeed;
            cannonBallY += SDL_sinf(cannonBallAngle) * cannonBallSpeed;
            SDL_Rect ballOffset{(int)(cannonBallX-cannonBallSource->w/2),(int)(cannonBallY-cannonBallSource->h/2),0,0};
            SDL_BlitSurface(cannonBallSource,nullptr,surface,&ballOffset);
            //check if it hit
            if (cannonBallY < targetY) {
                cannonShooting = false;
                blocks[targetBlockY*numberOfBlocksHorizontal + targetBlockX].animationProgress = 1;
                blocks[targetBlockY*numberOfBlocksHorizontal + targetBlockX].impactX = (int)targetX;
                blocks[targetBlockY*numberOfBlocksHorizontal + targetBlockX].impactY = (int)targetY;
            }
        }

        float cannonAngle = SDL_atan2f(mouseY - static_cast<float>(surface->h),mouseX - static_cast<float>(surface->w)/2.0f) * (180.0f/SDL_PI_F);
        SDL_Surface * unrotatedCannon = SDL_CreateSurface(200,80,SDL_PIXELFORMAT_RGBA8888);
        SDL_FillSurfaceRect(unrotatedCannon,nullptr,SDL_MapSurfaceRGB(unrotatedCannon,180,180,180));
        SDL_Surface * rotatedCannon = SDL_RotateSurface(unrotatedCannon, cannonAngle);
        SDL_Rect cannonOffset{surface->w/2-rotatedCannon->w/2,surface->h-rotatedCannon->h/2,0,0};
        SDL_BlitSurface(rotatedCannon,nullptr,surface,&cannonOffset);
        SDL_DestroySurface(unrotatedCannon);
        SDL_DestroySurface(rotatedCannon);

        SDL_UpdateWindowSurface(window);
        fr.delay();
    }

    delete[] blocks;
    delete[] image2HSV;
    SDL_DestroyWindow(window);
    SDL_DestroySurface(image2);
    SDL_DestroySurface(image1);
    SDL_DestroySurface(workingSurface);
    SDL_Quit();
    return 0;
}
