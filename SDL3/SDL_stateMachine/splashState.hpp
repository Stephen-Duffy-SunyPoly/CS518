#ifndef STATEMACHINE_SPLASHSTATE_HPP
#define STATEMACHINE_SPLASHSTATE_HPP
#include "qnd/initializer.hpp"
#include "qnd/stateMachine/State.hpp"

class SplashState: public qnd::stateMachine::State {
    SDL_Texture * background = nullptr;
public:
    explicit SplashState(qnd::stateMachine::Framework& framework, qnd::Initializer& init);
    bool handleEvent(SDL_Event& event) override;
    void draw(SDL_Renderer* renderer) override;
};
#endif //STATEMACHINE_SPLASHSTATE_HPP
