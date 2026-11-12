#ifndef QND_STATEMACHINE_STATE_HPP
#define QND_STATEMACHINE_STATE_HPP
#include "SDL3/SDL_render.h"

namespace qnd::stateMachine {
    class Framework;//forward declaration

    class State {
        protected:
            Framework * framework = nullptr;
        public:
            explicit State(Framework& framework);//, Initializer& init each specific state will likely need to take in the initializer, but it is not necessary here
            virtual bool handleEvent(SDL_Event& event) = 0;
            virtual void enter(){};
            virtual void leave(){};
            virtual void draw(SDL_Renderer * renderer) = 0;
            virtual void update([[maybe_unused]] double dt){};
            virtual ~State() = default;
    };
}
#endif //QND_STATEMACHINE_STATE_HPP
