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
    if (hasWindow) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
    for (int i=0;i<audioStreams.size();i++) {
        SDL_DestroyAudioStream(audioStreams[i]);
    }
    for (int i=0;i<audioBuffers.size();i++) {
        SDL_free(audioBuffers[i]);
    }
    for (int i=0;i<audioDevices.size();i++) {
        SDL_CloseAudioDevice(audioDevices[i]);
    }
    for (int i=0;i<mutexes.size();i++) {
        SDL_DestroyMutex(mutexes[i]);
    }
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

qnd::Initializer& qnd::Initializer::addWavAudio(const Uint8* data, const Uint64 dataSize, Uint8** audioDataOut, Uint32 * audioLengthOut, SDL_AudioSpec * audioSpecOut) {
    SDL_AudioSpec audioSpec;
    Uint8 * audiodata;
    Uint32 audiodataLength;

    SDL_IOStream * memoryIoStream = SDL_IOFromConstMem(data, dataSize);
    if (memoryIoStream == nullptr) {
        SDL_Log("Failed to allocate memory for SDL_IOFromConstMem: %s",SDL_GetError());
        cleanup();
        throw InitializeException();
    }
    if (!SDL_LoadWAV_IO(memoryIoStream,true, &audioSpec, &audiodata, &audiodataLength)) {
        SDL_Log("Failed to load WAV: %s",SDL_GetError());
        cleanup();
        throw InitializeException();
    }
    audioBuffers.add(audiodata);
    if (audioDataOut != nullptr) {
        *audioDataOut = audiodata;
    }
    if (audioLengthOut != nullptr) {
        *audioLengthOut = audiodataLength;
    }
    if (audioSpecOut != nullptr) {
        *audioSpecOut = audioSpec;
    }
    return *this;
}

qnd::Initializer& qnd::Initializer::addDefaultAudioPlaybackDevice(SDL_AudioDeviceID* audioDeviceOut) {
    SDL_AudioDeviceID device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (device == 0) {
        SDL_Log("Failed to open playback audio device: %s",SDL_GetError());
        cleanup();
        throw InitializeException();
    }
    audioDevices.add(device);
    if (audioDeviceOut != nullptr) {
        *audioDeviceOut = device;
    }
    return *this;
}

qnd::Initializer& qnd::Initializer::addDefaultAudioRecordingDevice(SDL_AudioDeviceID* audioDeviceOut) {
    SDL_AudioDeviceID device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_RECORDING, nullptr);
    if (device == 0) {
        SDL_Log("Failed to open recording audio device: %s",SDL_GetError());
        cleanup();
        throw InitializeException();
    }
    audioDevices.add(device);
    if (audioDeviceOut != nullptr) {
        *audioDeviceOut = device;
    }
    return *this;
}

qnd::Initializer& qnd::Initializer::addAudioStream(const SDL_AudioSpec* srcSpec, const SDL_AudioSpec* dstSpec, SDL_AudioStream** streamOut) {
    SDL_AudioStream * stream = SDL_CreateAudioStream(srcSpec,dstSpec);
    if (stream == nullptr) {
        SDL_Log("Failed to create audio stream: %s",SDL_GetError());
        cleanup();
        throw InitializeException();
    }
    audioStreams.add(stream);
    if (streamOut != nullptr) {
        *streamOut = stream;
    }
    return *this;
}

qnd::Initializer& qnd::Initializer::addMutex(SDL_Mutex** mutexOut) {
    SDL_Mutex * mutex = SDL_CreateMutex();
    if (mutexOut == nullptr) {
        SDL_Log("Failed to create mutex: %s",SDL_GetError());
        cleanup();
        throw InitializeException();
    }
    mutexes.add(mutex);
    if (mutexOut != nullptr) {
        *mutexOut = mutex;
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
