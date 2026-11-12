#include "Framework.hpp"
//for cleaning up the map
#include <ranges>
#include <SDL3/SDL_log.h>

#include "../frameRate.hpp"

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
    if (state == nullptr) {
        SDL_Log("Attempted to transition to null state!");
        return;
    }
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
    if (state == nullptr) {
        SDL_Log("Attempted to transition to null state!");
        return;
    }
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

void qnd::stateMachine::Framework::run() {
    bool shouldRun = true;
    qnd::FrameRate frameRate{};
    while (shouldRun) {
        SDL_Event event;
        //if multy step state handling was implement, this is where part 2 would go
        if (stateStack.size()==0) {
            SDL_Log("No current state found, quitting.");
            break;
        }
        //hanlde events
        while (SDL_PollEvent(&event)) {
            bool handled = false;
            //go through the stack trying to handle the event
            for (int i=static_cast<int>(stateStack.size())-1; i>=0; i--) {
                //if this state handled the event
                if (stateStack[i]->handleEvent(event)) {
                    //then don't continue trying the handle this event
                    handled = true;
                    break;
                }
            }
            if (handled){
                continue;
            }
            switch (event.type) {
                case SDL_EVENT_QUIT:
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    shouldRun = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_ESCAPE) {
                        shouldRun = false;
                    }
                    break;
                default:
                    break;
            }
        }

        //TODO add frame timeing to FrameRate
        stateStack.peak()->update(1);
        stateStack.peak()->draw(renderer);
        frameRate.delay();
    }
}

qnd::stateMachine::Framework::~Framework() {
    while (stateStack.size()!=0) {
        //leave each state on the stack
        stateStack.peak()->leave();
        stateStack.pop();
    }
    //delete the states
    for (const auto& val : states | std::views::values) {
        delete val;
    }
}
