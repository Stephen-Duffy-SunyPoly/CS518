#include "Framework.hpp"
//for cleaning up the map
#include <ranges>

#define MY_WINDOW_FLAGS (SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT)


qnd::stateMachine::Framework::Framework(const int width, const int height, const char* title, Initializer& init):
width(width), height(height){
    init.addWindow(title, width, height,MY_WINDOW_FLAGS, &window, &renderer);
}
void qnd::stateMachine::Framework::popTransition() {
    //for now, poping will not be counted as leaving the state
    stateStack.pop();
    SDL_FlushEvents(SDL_EVENT_FIRST, SDL_EVENT_LAST);
}

void qnd::stateMachine::Framework::transitionTo(State* state) {
    //it is worth knowing that this probably should be a 2 step process where this function sets some value that is acted upon later
    //leave the current state (if it exists)
    if (stateStack.size()!=0) {
        stateStack.peak()->leave();
    }
    SDL_FlushEvents(SDL_EVENT_FIRST, SDL_EVENT_LAST);
    //clear the transition stack
    stateStack.clear();
    //put the new state on the stack
    stateStack.push(state);
    //enter the new state
    state->enter();
}
void qnd::stateMachine::Framework::pushTransition(State* state) {
    SDL_FlushEvents(SDL_EVENT_FIRST, SDL_EVENT_LAST);
    //push the new state on the stack
    stateStack.push(state);
    //enter the state
    state->enter();
}

qnd::stateMachine::State* qnd::stateMachine::Framework::getState(const std::string& name) {
    return states[name];
}
qnd::stateMachine::State* qnd::stateMachine::Framework::getCurrentState() {
    return stateStack.peak();
}
void qnd::stateMachine::Framework::registerState(const std::string& name, State* state) {
    states.insert({name,state});
}
qnd::stateMachine::Framework::~Framework() {
    for (const auto& val : states | std::views::values) {
        delete val;
    }
}
