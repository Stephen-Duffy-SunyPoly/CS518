#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

constexpr int WIDTH = 300;
constexpr int HEIGHT = 300;

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

static Color randomColor() {
    return {
        static_cast<Uint8>(SDL_rand(255)),
        static_cast<Uint8>(SDL_rand(255)),
        static_cast<Uint8>(SDL_rand(255)),
    };
}

static Uint8 lerp(const Uint8 a, const Uint8 b, const double t) {
    return static_cast<Uint8>(
        (1 - t) * static_cast<double>(a) +
        t * static_cast<double>(b)
    );
}

static Color lerpColor(const Color a, const Color b, const double t) {
    return {
        lerp(a.r, b.r, t),
        lerp(a.g, b.g, t),
        lerp(a.b, b.b, t),
    };
}

static Color curveColor(const Color base, const Color support, const Color target, const double t) {
    Color first = lerpColor(base, support, t);
    Color second = lerpColor(support, target, t);
    return lerpColor(first, second, t);
}

int main(int argc, char* argv[]) {
    constexpr SDL_InitFlags INIT_FLAGS = SDL_INIT_VIDEO;

    //SDL initialization things
    if (!SDL_Init(INIT_FLAGS)) {
        SDL_Log("SDL could not initialize! SDL Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Prog 1: duffysd",WIDTH,HEIGHT,0);

    if (window == nullptr) {
        SDL_Log("Window could not be created! SDL Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface * surface = SDL_GetWindowSurface(window);
    if (surface == nullptr) {
        SDL_Log("Error obtaining window surface! SDL Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // my program initialization things

    double slideProgress = 0;
    Color base = randomColor();
    Color support = randomColor();
    Color target = randomColor();
    // my program initialization things
    SDL_Event e;
    SDL_Rect drawRect {
        0, 0, WIDTH, HEIGHT
    };
    while(SDL_PollEvent(&e) == false || e.type != SDL_EVENT_QUIT) {
        Color frameColor = curveColor(base, support, target, slideProgress);

        SDL_FillSurfaceRect(surface, &drawRect,SDL_MapSurfaceRGB(surface, frameColor.r, frameColor.g, frameColor.b));

        slideProgress += 0.00001;
        //if we have reached the target color
        if (slideProgress >= 1) {
            slideProgress = 0;
            base = target;
            //pick a new color
            support = randomColor();
            target = randomColor();
        }

        SDL_UpdateWindowSurface(window);
    }

    //clean up
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
