#include "menuState.hpp"
#include <assets.h>

#include "qnd/stateMachine/Framework.hpp"
#include "SDL3/SDL_log.h"
#include "qnd/math.hpp"

qnd::Font MenuState::loadFont(qnd::Initializer& init) {
    init.addTexture(font_22x32_png_data,font_22x32_png_size,&fontTexture,true);
    return qnd::Font(fontTexture);
}

MenuState::MenuState(qnd::stateMachine::Framework& framework, qnd::Initializer& init) :
font(loadFont(init)), State(framework) {}

bool MenuState::handleEvent(SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            SDL_FRect r{
                .x = 200,
                .y = 200,
                .w = 400,
                .h = 200,
            };
            SDL_FPoint p{
                .x = event.button.x, .y = event.button.y,
            };
            //if the point intersects the rectangle
            if (r^p) {
                framework->transitionTo(framework->getState("DBF"));
            }
            return true;
        }
    }else if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
            framework->popTransition();
            return  true;
        }
        if (event.key.key == SDLK_UP) {
            SDL_Log("UP");
            return true;
        }
    }
    return  false;
}
void MenuState::draw(SDL_Renderer* renderer) {
    if (splashState == nullptr) {
        splashState = framework->getState("splash");
    }
    splashState->draw(renderer);
    font.drawStringCentered(renderer,"Menu",1078/2,100);
    SDL_SetRenderDrawColor(renderer, 0x44, 0x66, 0x99,0xff);
    SDL_FRect r{
        .x = 200,
        .y = 200,
        .w = 400,
        .h = 200,
    };
    SDL_RenderFillRect(renderer,&r);
}
