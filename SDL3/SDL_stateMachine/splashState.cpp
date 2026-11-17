#include "splashState.hpp"
#include <assets.h>

#include "qnd/stateMachine/Framework.hpp"

qnd::SpriteSheet SplashState::loadSpriteSheet(qnd::Initializer& init) {
    init.addTexture(hero_sprite_sheet_png_data,hero_sprite_sheet_png_size,&spriteTexture);
    return {spriteTexture,6,4};
}

SplashState::SplashState(qnd::stateMachine::Framework& framework, qnd::Initializer& init) : sprite(loadSpriteSheet(init)) ,State(framework) {
    init.addTexture(shapes_png_data,shapes_png_size,&background);
}
bool SplashState::handleEvent([[maybe_unused]] SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_M) {
        framework->pushTransition(framework->getState("menu"));
        return true;
    }
    return false;
}
void SplashState::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer,background,nullptr,nullptr);
    sprite.draw(renderer,frameCnt%22, 50,50,1.0f);
    frameCnt++;
}
