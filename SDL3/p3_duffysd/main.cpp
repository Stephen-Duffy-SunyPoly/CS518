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
};

static int imap(const SDL_Surface * s, const int x, const int y) {
    int pixelsPerRow = static_cast<int>(s->pitch / sizeof(Uint32));
    return x+y*pixelsPerRow;
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



    SDL_BlitSurface(image1,nullptr,surface,nullptr);

    SDL_Event e;
    bool quit = false;

    FrameRate fr(60);
    //render loop
    while(!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT:
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    quit = true;
                    break;
                default:
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
            }
        }


        SDL_UpdateWindowSurface(window);
        fr.delay();
    }

    delete[] image2HSV;
    SDL_DestroyWindow(window);
    SDL_DestroySurface(image2);
    SDL_DestroySurface(image1);
    SDL_Quit();
    return 0;
}
