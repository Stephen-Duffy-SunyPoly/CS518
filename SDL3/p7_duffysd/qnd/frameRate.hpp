#ifndef SDL_RENDER_FRAMERATE_HPP
#define SDL_RENDER_FRAMERATE_HPP
#include <SDL3/SDL.h>

namespace qnd {
    class FrameRate {
    public:
        FrameRate(unsigned int fps = 30);
        void setFPS(unsigned int fps);
        unsigned int getFPS();
        Uint64 getFrameCount();

        void delay();
    private:
        unsigned fps_;
        Uint64 start_;
        Uint64 frameNanoSeconds_;
        Uint64 frameCount_=0;
    };
}

#endif //SDL_RENDER_FRAMERATE_HPP
