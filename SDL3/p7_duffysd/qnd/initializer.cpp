#include "initializer.hpp"

#include <SDL3_image/SDL_image.h>
#include "SDL3/SDL_log.h"

void qnd::Initializer::cleanup() {
    if (cleanedUp) {
        SDL_Log("Attempted to clean up initializer when already cleaned up!");
        throw InitializeException();
    }
    cleanedUp = true;
    for (Uint32 i=0;i<createdTextures.size();i++) {
        SDL_DestroyTexture(createdTextures[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

qnd::Initializer::Initializer(SDL_InitFlags flags) {
    if (!SDL_Init(flags)) {
        SDL_Log("Failed to initialize SDL %s",SDL_GetError());
        throw InitializeException();
    }
}

qnd::Initializer & qnd::Initializer::addWindow(const char *title, const int width, const int height, const SDL_WindowFlags window_flags,
    SDL_Window **windowOut, SDL_Renderer **rendererOut) {
    if (hasWindow) {
        SDL_Log("Window already created");
    } else {
        windowsWidth = width;
        windowsHeight = height;
        if (!SDL_CreateWindowAndRenderer(title, width, height, window_flags, &window, & renderer)) {
            SDL_Log("Failed to create window and renderer: %s",SDL_GetError());
            SDL_Quit();
            cleanedUp = true;
            throw InitializeException();
        }
        if (windowOut != nullptr) {
            *windowOut = window;
        }
        if (rendererOut != nullptr) {
            *rendererOut = renderer;
        }
        hasWindow = true;
    }
    return *this;
}

qnd::Initializer & qnd::Initializer::addWindowSizeOfTexture(const char *title, const SDL_WindowFlags window_flags,
    SDL_Window **windowOut, SDL_Renderer **rendererOut, const Uint8 *imageData, const Uint64 imageDataSize,
    SDL_Texture **textureOut) {
    if (hasWindow) {
        SDL_Log("Window already created");
        SDL_Quit();
        cleanedUp = true;
        throw InitializeException();
    }
    SDL_IOStream * memoryIoStream = SDL_IOFromConstMem(imageData, imageDataSize);
    if (memoryIoStream == nullptr) {
        SDL_Log("Failed to allocate memory for SDL_IOFromConstMem: %s",SDL_GetError());
        SDL_Quit();
        cleanedUp = true;
        throw InitializeException();
    }
    SDL_Surface * imgSurface = IMG_Load_IO(memoryIoStream,true);
    if (imgSurface == nullptr) {
        SDL_Log("Failed to load Image: %s",SDL_GetError());
        SDL_Quit();
        cleanedUp = true;
        throw InitializeException();
    }
    windowsWidth = imgSurface->w;
    windowsHeight = imgSurface->h;
    if (!SDL_CreateWindowAndRenderer(title, windowsWidth, windowsHeight, window_flags, &window, & renderer)) {
        SDL_Log("Failed to create window and renderer: %s",SDL_GetError());
        SDL_Quit();
        cleanedUp = true;
        throw InitializeException();
    }
    if (windowOut != nullptr) {
        *windowOut = window;
    }
    if (rendererOut != nullptr) {
        *rendererOut = renderer;
    }
    hasWindow = true;
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, imgSurface);
    if (texture == nullptr) {
        SDL_Log("Failed to create Texture: %s",SDL_GetError());
        cleanup();
        throw InitializeException();
    }
    SDL_DestroySurface(imgSurface);
    //now to add it to the list of textures we know  about
    createdTextures.add(texture);
    if (textureOut != nullptr) {
        *textureOut = texture;
    }
    return *this;
}

qnd::Initializer & qnd::Initializer::addTexture(const Uint8 *data, const Uint64 dataSize, SDL_Texture **textureOut, bool useColorKey) {
    if (!hasWindow) {
        SDL_Log("creating textures requires the window to be already created");
        SDL_Quit();
        cleanedUp = true;
        throw InitializeException();
    }
    SDL_IOStream * memoryIoStream = SDL_IOFromConstMem(data, dataSize);
    if (memoryIoStream == nullptr) {
        SDL_Log("Failed to allocate memory for SDL_IOFromConstMem: %s",SDL_GetError());
        cleanup();
        throw InitializeException();
    }
    SDL_Surface * imgSurface = IMG_Load_IO(memoryIoStream,true);
    if (imgSurface == nullptr) {
        SDL_Log("Failed to load Image: %s",SDL_GetError());
        cleanup();
        throw InitializeException();
    }
    if (useColorKey) {
        SDL_SetSurfaceColorKey(imgSurface, true, *static_cast<Uint32 *>(imgSurface->pixels));
    }
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, imgSurface);
    if (texture == nullptr) {
        SDL_Log("Failed to create Texture: %s",SDL_GetError());
        cleanup();
        throw InitializeException();
    }
    SDL_DestroySurface(imgSurface);
    //now to add it to the list of textures we know  about
    createdTextures.add(texture);
    if (textureOut != nullptr) {
        *textureOut = texture;
    }
    return *this;
}

int qnd::Initializer::getWindowWidth() const {
    return windowsWidth;
}

int qnd::Initializer::getWindowHeight() const {
    return windowsHeight;
}

qnd::Initializer::~Initializer() {
    if (!cleanedUp) {
        cleanup();
    }
}
