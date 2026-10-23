#ifndef INITIALIZER_HPP
#define INITIALIZER_HPP

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>

class Initializer {
  bool cleanedUp = false;
  bool hasWindow = false;
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Texture ** createdTextures = nullptr;
  Uint32 numberOfTextures = 0;
  Uint32 allocatedTextures = 0;
  int windowsWidth = 0;
  int windowsHeight = 0;

  void cleanup();
  void allocateMoreSpace();
  void addTextureToList(SDL_Texture *texture);

public:
  explicit Initializer(SDL_InitFlags flags);

  Initializer & addWindow(const char *title, int width, int height,SDL_WindowFlags window_flags, SDL_Window **windowOut, SDL_Renderer **rendererOut);
  Initializer & addWindowSizeOfTexture(const char *title, SDL_WindowFlags window_flags, SDL_Window **windowOut, SDL_Renderer **rendererOut, const Uint8 * imageData, Uint64 imageDataSize, SDL_Texture ** textureOut);
  Initializer & addTexture(const Uint8 * data, Uint64 dataSize, SDL_Texture ** textureOut, bool useColorKey = false);

  int getWindowWidth() const;
  int getWindowHeight() const;

  ~Initializer();
};

struct InitializeException {};

#endif //INITIALIZER_HPP
