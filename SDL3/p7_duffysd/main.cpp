#include <SDL3/SDL.h>
#include <assets.h>
#include "qnd/frameRate.hpp"
#include "qnd/spriteSheet.hpp"
#include "qnd/initializer.hpp"
#include "qnd/math.hpp"

static bool raceStarted = false;
static bool raceFinished = false;
static int winner = 0;
static SDL_Mutex * raceFinishedMutex = nullptr;

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
    for (int i=0;i<STEPS+1 && !raceFinished;i++) {
        racerProgress[racer] = static_cast<float>(i) / static_cast<float>(STEPS);
        SDL_DelayNS(SDL_rand(15000000));//seems like a serviceable amount of random delay
    }
    //acquire the mutex
    SDL_LockMutex(raceFinishedMutex);
    if (!raceFinished) {
        winner = racer;
        raceFinished = true;
    }
    SDL_UnlockMutex(raceFinishedMutex);
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
    SDL_Texture * celebrationStarTexture = nullptr;

    SDL_Texture * racers[4];

    qnd::WavAudioInfo startAudioInfo{};
    qnd::WavAudioInfo raceMusicInfo{};
    qnd::WavAudioInfo raceFinishAudioInfo{};


    SDL_AudioDeviceID audioDevice;
    SDL_AudioStream * audioStream;
    SDL_AudioStream * endCelebrateAudioStream;

    init.addWindowSizeOfTexture("p6_duffysd",0, &window, &renderer,track_png_data, track_png_size, &background)
    .addTexture(font_11x16_png_data, font_11x16_png_size, &fontSmallTexture,true)
    .addTexture(font_22x32_png_data, font_22x32_png_size, &fontLargeTexture,true)
    .addWavAudio(start_wav_data, start_wav_size, &startAudioInfo)
    .addWavAudio(Main_Theme_wav_data, Main_Theme_wav_size, &raceMusicInfo)
    .addWavAudio(complete_wav_data, complete_wav_size, &raceFinishAudioInfo)
    .addDefaultAudioPlaybackDevice(&audioDevice)
    .addAudioStream(&startAudioInfo.spec, nullptr, &audioStream)
    .addAudioStream(&raceFinishAudioInfo.spec, nullptr,&endCelebrateAudioStream)
    .addMutex(&raceFinishedMutex)
    .addTexture(racer1_png_data,racer1_png_size, &racers[0])
    .addTexture(racer2_png_data,racer2_png_size, &racers[1])
    .addTexture(racer3_png_data,racer3_png_size, &racers[2])
    .addTexture(racer4_png_data,racer4_png_size, &racers[3])
    .addTexture(star_png_data,star_png_size, &celebrationStarTexture)
    ;




    qnd::Font fontSmall(fontSmallTexture);
    qnd::Font fontLarge(fontLargeTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_BindAudioStream(audioDevice,audioStream);
    SDL_BindAudioStream(audioDevice,endCelebrateAudioStream);
    SDL_PutAudioStreamData(audioStream, startAudioInfo.data,static_cast<int>(startAudioInfo.dataSize));

    //create the threads
    SDL_Thread * threads[4];
    for (int i=0;i<4;i++) {
        threads[i] = SDL_CreateThread(racerThread, "racerThread", reinterpret_cast<void*>(static_cast<intptr_t>(i)));
    }
    int easeOptions[] = {0,1,2,3,4,5};
    for (int i=0;i<4;i++) {
        int option = SDL_rand(6-i);
        easingIndices[i] = easeOptions[option];
        for (int j = option; j< 5;j++) {
            easeOptions[j] = easeOptions[j+1];
        }
    }

    bool should_quit = false;
    bool finishAnimationStarted = false;
    int finishedAnimationFrame = 0 ;
    int fadeUpStart = 0;
    qnd::FrameRate frameRate;

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

        if (!raceStarted) {
            if (SDL_GetAudioStreamAvailable(audioStream) == 0) {
                SDL_SetAudioStreamFormat(audioStream,&raceMusicInfo.spec,nullptr);
                SDL_PutAudioStreamData(audioStream,raceMusicInfo.data,static_cast<int>(raceMusicInfo.dataSize));
                raceStarted = true;
            }
        } else if (raceFinished && !finishAnimationStarted) {
            finishAnimationStarted = true;
            SDL_PutAudioStreamData(endCelebrateAudioStream, raceFinishAudioInfo.data,static_cast<int>(raceFinishAudioInfo.dataSize));
            SDL_SetAudioStreamGain(audioStream, 0.3f);
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
                .w = 80,
                .h = 80,
            };
            spritePos.x -= spritePos.w;

            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            SDL_RenderTexture(renderer, racers[i], nullptr, &spritePos);
        }

        if (raceFinished) {
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);
            SDL_RenderFillRect(renderer, nullptr);
            fontLarge.drawStringCentered(renderer, "Your", init.getWindowWidth()/2,100);
            if (finishedAnimationFrame > 25) {
                fontLarge.drawStringCentered(renderer, "Winner", init.getWindowWidth()/2,150);
            }
            if (finishedAnimationFrame > 47) {
                fontLarge.drawStringCentered(renderer, "IS", init.getWindowWidth()/2,200);
            }
            if (finishedAnimationFrame > 54) {
                SDL_FRect starRect{
                  .x = static_cast<float>(init.getWindowWidth() / 2 - 350),
                  .y = 100,
                  .w = 700,
                  .h = 700,
                };
                SDL_RenderTextureRotated(renderer, celebrationStarTexture,
                    nullptr, &starRect,
                    finishedAnimationFrame, nullptr, SDL_FLIP_NONE);

                SDL_FRect tr{
                    .x= static_cast<float>(init.getWindowWidth() / 2 - 335+ ((winner == 3)?-170:0)),
                    .y = 250.0f,
                    .w = 600,
                    .h = 400,
                };
                SDL_RenderTexture(renderer, racers[winner], nullptr, &tr);
                fontSmall.drawStringCentered(renderer,"no refunds", init.getWindowWidth()/2,700);
            }
            if (fadeUpStart == 0 && SDL_GetAudioStreamAvailable(endCelebrateAudioStream) == 0) {
                fadeUpStart = finishedAnimationFrame;
            }

            if (fadeUpStart != 0 && finishedAnimationFrame >= fadeUpStart && finishedAnimationFrame <= fadeUpStart+30) {
                const int ufr = finishedAnimationFrame - fadeUpStart;
                const float per = static_cast<float>(ufr) / 30.0f;
                SDL_SetAudioStreamGain(audioStream, lerp(0.3f,1.0f,per));
            }

            finishedAnimationFrame++;
        }


        SDL_RenderPresent(renderer);
        frameRate.delay();

    }

    //clean up zone
    raceFinished = true;
    raceStarted = true;

    for (auto & thread : threads) {
        SDL_WaitThread(thread, nullptr);
    }

    return 0;
}