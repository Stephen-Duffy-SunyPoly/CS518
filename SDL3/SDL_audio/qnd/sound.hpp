#ifndef QND_SOUND_HPP
#define QND_SOUND_HPP
#include "initializer.hpp"
namespace qnd {
    class Sound {
        WavAudioInfo audioInfo{};
        bool loop;
        SDL_AudioDeviceID deviceID{};
        SDL_AudioStream * stream = nullptr;
        Uint32 bufferLoopOffset = 0;

        static void SDLCALL soundLoopCallBack(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount);
    public:
        Sound(Initializer &init, const Uint8 * fileData, Uint64 fileSize, bool loop,bool startPaused=false);
        ~Sound();

        void replay() const;

    };
}
#endif //QND_SOUND_HPP
