#ifndef STATEMACHINE_SPLASHSTATE_HPP
#define STATEMACHINE_SPLASHSTATE_HPP
#include "qnd/initializer.hpp"
#include "qnd/spriteSheet.hpp"
#include "qnd/stateMachine/State.hpp"

class SplashState: public qnd::stateMachine::State {
    SDL_Texture * background = nullptr;
    SDL_Texture * spriteTexture = nullptr;
    qnd::SpriteSheet sprite;
    int frameCnt = 0;
    qnd::SpriteSheet loadSpriteSheet(qnd::Initializer& init);
public:
    explicit SplashState(qnd::stateMachine::Framework& framework, qnd::Initializer& init);
    bool handleEvent(SDL_Event& event) override;
    void draw(SDL_Renderer* renderer) override;
};
#endif //STATEMACHINE_SPLASHSTATE_HPP
