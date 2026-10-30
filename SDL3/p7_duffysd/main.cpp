#include <SDL3/SDL.h>
#include <assets.h>
#include "qnd/frameRate.hpp"
#include "qnd/spriteSheet.hpp"
#include "qnd/initializer.hpp"
#include "qnd/math.hpp"

bool raceStarted = false;
bool raceFinished = false;
int winner = 0;

constexpr int STEPS = 2*2*2*2*2*2*2*2*2*2*2*2;
static float racerProgress[4] = {0,0,0,0};
static int easingIndices[4] = {0,0,0,0};

static float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

static float (*easingFunctions[])(float) = {
    [](const float t){return t + SDL_sinf(25.2f*t)/25.2f;},
    [](const float t){return SDL_powf(t,0.15f);},
    [](const float t){return SDL_powf(t,20.f);},
    [](const float t){return (SDL_sinf(t*SDL_PI_F - SDL_PI_F/2.0f)+1.0f)/2.0f;},
    [](const float t) {
        float x = t*6;
        float h = x + 3*SDL_expf(-SDL_powf(x-3.0f,2.0f)) * SDL_sinf(4.0f*x);
        return h/6.0f;
    },
    [](const float t) {
        float x = t*6;
        float h = x + 3*SDL_expf(-SDL_powf(x-3.0f,2.0f)) * SDL_sinf(15.0f*x);
        float i = h/6.0f;
        float x2 = 40*t-20;
        float cmb = SDL_expf(-x2)/SDL_powf(1.0f+SDL_expf(-x2),2.0f);
        cmb/=4.0f;
        float d = t;
        float l = lerp(i,d,cmb);
        return l;
    }
};

static int SDLCALL racerThread(void * data) {
    int racer = static_cast<int>(reinterpret_cast<intptr_t>(data));
    while (!raceStarted) {
        //spin wait
    }
    for (int i=0;i<STEPS && !raceFinished;i++) {
        racerProgress[racer] = static_cast<float>(i) / static_cast<float>(STEPS);
        SDL_DelayNS(SDL_rand(15000000));//seems like a serviceable amount of random delay
    }
    //acquire the mutex
    if (!raceFinished) {
        winner = racer;
        raceFinished = true;
    }
    //release the mutex
    return 0;
}

int main() {
    qnd::Initializer init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    SDL_Texture * background = nullptr;
    SDL_Texture * fontSmallTexture = nullptr;
    SDL_Texture * fontLargeTexture = nullptr;

    Uint8 * testWavData = nullptr;
    Uint32 wavDataSize =0;
    SDL_AudioSpec wavDataSpec;

    SDL_AudioDeviceID audioDevice;
    SDL_AudioStream * audioStream;

    init.addWindowSizeOfTexture("p6_duffysd",0, &window, &renderer,track_png_data, track_png_size, &background)
    .addTexture(font_11x16_png_data, font_11x16_png_size, &fontSmallTexture,true)
    .addTexture(font_22x32_png_data, font_22x32_png_size, &fontLargeTexture,true)
    .addWavAudio(Level_Complete_Sound_Effect_wav_data, Level_Complete_Sound_Effect_wav_size, &testWavData, &wavDataSize, & wavDataSpec)
    .addDefaultAudioPlaybackDevice(&audioDevice)
    .addAudioStream(&wavDataSpec, nullptr, &audioStream)
    ;

    // const int width = init.getWindowWidth();
    // const int height = init.getWindowHeight();


    qnd::Font fontSmall(fontSmallTexture);
    qnd::Font fontLarge(fontLargeTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // SDL_BindAudioStream(audioDevice,audioStream);
    // SDL_PutAudioStreamData(audioStream, testWavData,static_cast<int>(wavDataSize));

    //create the threads
    SDL_Thread * threads[4];
    for (int i=0;i<4;i++) {
        threads[i] = SDL_CreateThread(racerThread, "racerThread", reinterpret_cast<void*>(static_cast<intptr_t>(i)));
    }

    bool should_quit = false;
    qnd::FrameRate frameRate;
    raceStarted = true; //tmp
    while (!should_quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    should_quit = true;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch (event.key.key) {
                        case SDLK_ESCAPE:
                            should_quit = true;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        //draw the background
        SDL_RenderTexture(renderer, background,nullptr,nullptr);

        //left edge is 152
        //right edge is 1213
        const float racerYs[] = {160, 275, 390, 500};

        constexpr float trackWidth = 1213 - 152;

        for (int i=0;i<4;i++) {
            SDL_FRect spritePos{
                .x = easingFunctions[easingIndices[i]](racerProgress[i]) * trackWidth+152,
                .y = racerYs[i],
                .w = 50,
                .h = 50,
            };
            spritePos.x -= spritePos.w;

            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            SDL_RenderFillRect(renderer, &spritePos);
        }




        SDL_RenderPresent(renderer);
        frameRate.delay();

    }

    //clean up zone
    raceFinished = true;

    for (auto & thread : threads) {
        SDL_WaitThread(thread, nullptr);
    }

    return 0;
}