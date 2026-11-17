#ifndef STATEMACHINE_MENUSTATE_HPP
#define STATEMACHINE_MENUSTATE_HPP
#include "qnd/stateMachine/state.hpp"
#include "qnd/initializer.hpp"
#include "qnd/spriteSheet.hpp"


class MenuState : public qnd::stateMachine::State {
    SDL_Texture * fontTexture = nullptr;
    qnd::Font font;
    qnd::Font loadFont(qnd::Initializer& init);
    qnd::stateMachine::State * splashState = nullptr;
    public:
    explicit MenuState(qnd::stateMachine::Framework & framework, qnd::Initializer& init);
    bool handleEvent(SDL_Event& event) override;
    void draw(SDL_Renderer* renderer) override;
};
#endif //STATEMACHINE_MENUSTATE_HPP
