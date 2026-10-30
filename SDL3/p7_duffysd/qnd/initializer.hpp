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

    int windowsWidth = 0;
    int windowsHeight = 0;

    void cleanup();;

  public:
    explicit Initializer(SDL_InitFlags flags);

    Initializer & addWindow(const char *title, int width, int height,SDL_WindowFlags window_flags, SDL_Window **windowOut, SDL_Renderer **rendererOut);
    Initializer & addWindowSizeOfTexture(const char *title, SDL_WindowFlags window_flags, SDL_Window **windowOut, SDL_Renderer **rendererOut, const Uint8 * imageData, Uint64 imageDataSize, SDL_Texture ** textureOut);
    Initializer & addTexture(const Uint8 * data, Uint64 dataSize, SDL_Texture ** textureOut, bool useColorKey = false);
    Initializer & addWavAudio(const Uint8 * data, Uint64 dataSize, Uint8 ** audioDataOut, Uint32 * audioLengthOut);

    [[nodiscard]] int getWindowWidth() const;
    [[nodiscard]] int getWindowHeight() const;

    ~Initializer();
  };

  struct InitializeException {};
}
#endif //INITIALIZER_HPP
