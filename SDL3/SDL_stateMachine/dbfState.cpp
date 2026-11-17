#include "dbfState.hpp"

#include "qnd/stateMachine/Framework.hpp"
#include "SDL3/SDL_log.h"
DbfState::DbfState(qnd::stateMachine::Framework& framework) : State(framework) {}
bool DbfState::handleEvent(SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN ) {
        if (event.key.key == SDLK_ESCAPE) {
            framework->transitionTo(framework->getState("splash"));
            return true;
        }
        if (event.key.key == SDLK_UP) {
            SDL_Log("fabulous");
            return true;
        }
    }
    return false;
}
void DbfState::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0xDD, 0xBB, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
}
