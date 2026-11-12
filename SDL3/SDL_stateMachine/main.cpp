#include <iostream>
#include <SDL3/SDL.h>
#include "qnd/initializer.hpp"

#include "qnd/stateMachine/Framework.hpp"
#include "splashState.hpp"

constexpr SDL_InitFlags initFLags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {

    qnd::Initializer init(initFLags);

    qnd::stateMachine::Framework framework(1078,604,"Framework example",init);

    //create and register states
    framework.registerState("splash",new SplashState(framework,init));

    //transition to the first state
    framework.transitionTo(framework.getState("splash"));//splash screen
    //run the program
    framework.run();

    return EXIT_SUCCESS;
}