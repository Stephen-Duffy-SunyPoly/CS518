#ifndef STATEMACHINE_DBFSTATE_HPP
#define STATEMACHINE_DBFSTATE_HPP
#include "qnd/stateMachine/State.hpp"


class DbfState: public qnd::stateMachine::State {
    public:
    explicit DbfState(qnd::stateMachine::Framework & framework);
    bool handleEvent(SDL_Event& event) override;
    void draw(SDL_Renderer* renderer) override;
};
#endif //STATEMACHINE_DBFSTATE_HPP
