#include "splashState.hpp"
#include <assets.h>
SplashState::SplashState(qnd::stateMachine::Framework& framework, qnd::Initializer& init) : State(framework) {
    init.addTexture(shapes_png_data,shapes_png_size,&background);
}
bool SplashState::handleEvent([[maybe_unused]] SDL_Event& event) {
    return false;
}
void SplashState::draw(SDL_Renderer* renderer) {
    SDL_RenderTexture(renderer,background,nullptr,nullptr);
    SDL_RenderPresent(renderer);
}
