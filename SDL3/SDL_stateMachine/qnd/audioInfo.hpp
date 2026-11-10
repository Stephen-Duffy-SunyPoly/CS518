#ifndef QND_AUDIOINFO_HPP
#define QND_AUDIOINFO_HPP
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_audio.h>
namespace qnd {
    struct WavAudioInfo {
        Uint8 * data;
        Uint32 dataSize;
        SDL_AudioSpec spec;
    };
}
#endif //QND_AUDIOINFO_HPP
