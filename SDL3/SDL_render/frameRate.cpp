#include <SDL3/SDL.h>
#include "frameRate.hpp"

FrameRate::FrameRate(unsigned int fps) {
    setFPS(fps);
}

void FrameRate::setFPS(unsigned int fps) {
    fps_ = fps;
    if (fps == 0) {
        return;
    }
    frameNanoSeconds_ = 1'000'000'000/fps;
    start_ = SDL_GetTicksNS();
}

unsigned int FrameRate::getFPS() {
    return fps_;
}

void FrameRate::delay() {
    if (fps_ == 0) {
        return;
    }
    Uint64 toDelay = frameNanoSeconds_ - (SDL_GetTicksNS() - start_);
    SDL_DelayPrecise(toDelay);
    start_ += frameNanoSeconds_;
}
