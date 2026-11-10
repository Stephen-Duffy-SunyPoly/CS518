
#ifndef QND_STATEMACHINE_FRAMEWORK_HPP
#define QND_STATEMACHINE_FRAMEWORK_HPP
#include <SDL3/SDL_stdinc.h>
#include <map>
#include <string>

#include "State.hpp"
#include "../initializer.hpp"
#include "../stack.hpp"
#include "SDL3/SDL_render.h"

namespace qnd::stateMachine {

    class Framework {
        int width, height;
        Stack<State *> stateStack;
        SDL_Renderer *renderer = nullptr;
        SDL_Window *window = nullptr;
        std::map<std::string, State *> states;
    public:
        Framework(int width, int height, const char* title, Initializer& init);
        void popTransition();
        State * getState(const std::string& name);
        State * getCurrentState();
        void registerState(const std::string& name, State *state);
        ~Framework();
    };
}
#endif //QND_STATEMACHINE_FRAMEWORK_HPP
