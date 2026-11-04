#include <iostream>
#include <SDL3/SDL.h>
#include "qnd/frameRate.hpp"
#include "qnd/initializer.hpp"
#include <assets.h>

constexpr SDL_InitFlags initFLags = SDL_INIT_VIDEO;

static void SDLCALL loopCallback(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount) {
    std:: cerr << "Loop Callback additional_amount " << additional_amount << " total_amount "<< total_amount <<std::endl;
}

int main(int argc, char* argv[]) {

    qnd::Initializer init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);


    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    SDL_Texture * background = nullptr;
    SDL_AudioStream * audioStream = nullptr;
    SDL_AudioDeviceID audioDevice;
    qnd::WavAudioInfo audioLoop{};

    init.addWindowSizeOfTexture("SDL Audio Demo", SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT,
                                &window ,&renderer, shapes_png_data, shapes_png_size, &background)
        .addDefaultAudioPlaybackDevice(&audioDevice)
        .addWavAudio(track3_wav_data, track3_wav_size, &audioLoop)
        .addAudioStream(&audioLoop.spec, nullptr, &audioStream);

    SDL_BindAudioStream(audioDevice, audioStream);

    SDL_SetAudioStreamGetCallback(audioStream, loopCallback,nullptr);


    SDL_PutAudioStreamData(audioStream, audioLoop.data, static_cast<int>(audioLoop.dataSize));

    qnd::FrameRate fr(30);

    bool shouldRun = true;
    while (shouldRun) {
        SDL_Event event;
        //read all events
        while (SDL_PollEvent(&event)) {
            //handle the event
            int newWidth;
            int newHeight;
            switch (event.type) {
            case SDL_EVENT_QUIT:
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                shouldRun = false;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                newWidth = event.window.data1;
                newHeight = event.window.data2;
                std::cout << "Resized: " << newWidth << "x" << newHeight << std::endl;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        shouldRun = false;
                    break;
                case SDLK_1:
                    SDL_ClearAudioStream(audioStream);
                    SDL_PutAudioStreamData(audioStream, audioLoop.data, static_cast<int>(audioLoop.dataSize));
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_RenderTexture(renderer, background, nullptr, nullptr);

        // if (SDL_GetAudioStreamAvailable(audioStream) == 0) {
        //     SDL_PutAudioStreamData(audioStream, audioLoop.data, static_cast<int>(audioLoop.dataSize));
        // }

        SDL_RenderPresent(renderer);
        fr.delay();
    }

    return EXIT_SUCCESS;
}