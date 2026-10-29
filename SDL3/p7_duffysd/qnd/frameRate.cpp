#include <SDL3/SDL.h>
#include "frameRate.hpp"

qnd::FrameRate::FrameRate(unsigned int fps) { // NOLINT(*-pro-type-member-init)
    setFPS(fps);
}

void qnd::FrameRate::setFPS(unsigned int fps) {
    fps_ = fps;
    if (fps == 0) {
        return;
    }
    frameNanoSeconds_ = 1'000'000'000/fps;
    start_ = SDL_GetTicksNS();
}

unsigned int qnd::FrameRate::getFPS() {
    return fps_;
}

Uint64 qnd::FrameRate::getFrameCount() {
    return frameCount_;
}

void qnd::FrameRate::delay() {
    if (fps_ == 0) {
        return;
    }
    Uint64 elapsed = SDL_GetTicksNS() - start_;
    Uint64 toDelay = frameNanoSeconds_ - elapsed;
    if (frameNanoSeconds_ > elapsed) {
        SDL_DelayPrecise(toDelay);
    }
    start_ += frameNanoSeconds_;
    frameCount_ ++;
}
