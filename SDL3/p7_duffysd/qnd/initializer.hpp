#ifndef INITIALIZER_HPP
#define INITIALIZER_HPP

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include "list.hpp"

namespace qnd {
  class Initializer {
    bool cleanedUp = false;
    bool hasWindow = false;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    List<SDL_Texture *> createdTextures;
    List<Uint8*> audioBuffers;
    List<SDL_AudioDeviceID> audioDevices;
    List<SDL_AudioStream *> audioStreams;

    int windowsWidth = 0;
    int windowsHeight = 0;

    void cleanup();;

  public:
    explicit Initializer(SDL_InitFlags flags);

    Initializer & addWindow(const char *title, int width, int height,SDL_WindowFlags window_flags, SDL_Window **windowOut, SDL_Renderer **rendererOut);
    Initializer & addWindowSizeOfTexture(const char *title, SDL_WindowFlags window_flags, SDL_Window **windowOut, SDL_Renderer **rendererOut, const Uint8 * imageData, Uint64 imageDataSize, SDL_Texture ** textureOut);
    Initializer & addTexture(const Uint8 * data, Uint64 dataSize, SDL_Texture ** textureOut, bool useColorKey = false);
    Initializer & addWavAudio(const Uint8 * data, Uint64 dataSize, Uint8 ** audioDataOut, Uint32 * audioLengthOut, SDL_AudioSpec * audioSpecOut);
    Initializer & addDefaultAudioPlaybackDevice(SDL_AudioDeviceID * audioDeviceOut);
    Initializer & addDefaultAudioRecordingDevice(SDL_AudioDeviceID * audioDeviceOut);
    /**
     * @param srcSpec The audio spec of the source, can be null
     * @param dstSpec The audio spec of the destination, can be null
     * @param streamOut The stream being returned
     * @return this
     */
    Initializer & addAudioStream(const SDL_AudioSpec * srcSpec, const SDL_AudioSpec * dstSpec, SDL_AudioStream ** streamOut);


    [[nodiscard]] int getWindowWidth() const;
    [[nodiscard]] int getWindowHeight() const;

    ~Initializer();
  };

  struct InitializeException {};
}
#endif //INITIALIZER_HPP
