
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
        Stack<State *> stateStack;
        std::map<std::string, State *> states;//holds the canonical reference to these pointers
        int width, height;
        SDL_Renderer *renderer = nullptr;
        SDL_Window *window = nullptr;

    public:
        Framework(int width, int height, const char* title, Initializer& init);
        /**Go to a new state, disposing of any currently stacked states
         * @param state the new state to go to
         */
        void transitionTo(State *state);
        /**Go to a new state and push it onto the state stack.
         * @param state the new state to go to
         */
        void pushTransition(State *state);
        /**Go back to the prevous state, poping the current state from the stack
         */
        void popTransition();
        State * getState(const std::string& name);
        State * getCurrentState();
        void registerState(const std::string& name, State *state);
        ~Framework();
    };
}
#endif //QND_STATEMACHINE_FRAMEWORK_HPP
