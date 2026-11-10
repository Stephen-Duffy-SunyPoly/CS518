#include "Framework.hpp"
//for cleaning up the map
#include <ranges>

#define MY_WINDOW_FLAGS (SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT)


qnd::stateMachine::Framework::Framework(const int width, const int height, const char* title, Initializer& init):
width(width), height(height){
    init.addWindow(title, width, height,MY_WINDOW_FLAGS, &window, &renderer);
}
void qnd::stateMachine::Framework::popTransition() {
    stateStack.pop();
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
