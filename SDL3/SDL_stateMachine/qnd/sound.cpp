#include "sound.hpp"

void SDLCALL qnd::Sound::soundLoopCallBack(void* userdata, SDL_AudioStream* stream, int additional_amount,
                                           [[maybe_unused]] int total_amount) {
    auto * sound = static_cast<Sound *>(userdata);
    if (sound->loop) {
        Uint32 chunkSize;
        while (additional_amount > 0) {
            //get the amount of data in the buffer that has yet to be played
            Uint32 remaining = sound->audioInfo.dataSize - sound->bufferLoopOffset;
            //if the amount of data request would run past the end of the buffer, set chink size to the amount of data that remains
            chunkSize = SDL_min((Uint32)additional_amount, remaining);
            //send the chunk of data to the buffer
            SDL_PutAudioStreamData(stream, sound->audioInfo.data + sound->bufferLoopOffset, static_cast<int>(chunkSize));
            //update the buffer offset
            sound->bufferLoopOffset += chunkSize;
            sound->bufferLoopOffset %= sound->audioInfo.dataSize;
            additional_amount -= static_cast<int>(chunkSize);
        }
    }
}

qnd::Sound::Sound(Initializer& init, const Uint8* fileData, const Uint64 fileSize, const bool loop, const bool startPaused)
    :loop(loop) {
    init.addWavAudio(fileData,fileSize,&audioInfo)
    .addDefaultAudioPlaybackDevice(&deviceID)
    .addAudioStream(&audioInfo.spec,nullptr,&stream);
    SDL_BindAudioStream(deviceID,stream);
    if (startPaused) {
        SDL_PauseAudioDevice(deviceID);
    }
    if (loop) {
        SDL_SetAudioStreamGetCallback(stream, soundLoopCallBack,(void *)this);
    }
}

//current dynamic allocations and destruction are handled by the initializer, do no need to do anything here
qnd::Sound::~Sound() = default;

void qnd::Sound::replay() const {
    SDL_ClearAudioStream(stream);
    SDL_PutAudioStreamData(stream,audioInfo.data,static_cast<int>(audioInfo.dataSize));
    SDL_ResumeAudioDevice(deviceID);
}
