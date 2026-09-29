#ifndef SDL_RENDER_FRAMERATE_HPP
#define SDL_RENDER_FRAMERATE_HPP
#include <SDL3/SDL.h>

class FrameRate {
    public:
        FrameRate(unsigned int fps = 30);
        void setFPS(unsigned int fps);
        unsigned int getFPS();

        void delay();
    private:
        unsigned fps_;
        Uint64 start_;
        Uint64 frameNanoSeconds_;
};

#endif //SDL_RENDER_FRAMERATE_HPP
