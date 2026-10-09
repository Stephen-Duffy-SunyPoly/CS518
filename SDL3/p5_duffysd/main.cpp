#include <SDL3/SDL.h>
#include <SDL3_gfxPrimitives.h>

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window * window;
    SDL_Renderer * renderer;

    if (!SDL_CreateWindowAndRenderer("p5_duffysd",1000,500,0,&window, &renderer)) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return 1;
    }

    bool should_quit = false;

    Sint16 a = 305,b=175,c=20,d=10;

    while (!should_quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    should_quit = true;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch (event.key.key) {
                        case SDLK_ESCAPE:
                            should_quit = true;
                            break;
                        case SDLK_1:
                            a--;
                            SDL_Log("%d, %d, %d, %d",a,b,c,d);
                            break;
                        case SDLK_2:
                            a++;
                            SDL_Log("%d, %d, %d, %d",a,b,c,d);
                            break;
                        case SDLK_3:
                            b--;
                            SDL_Log("%d, %d, %d, %d",a,b,c,d);
                            break;
                        case SDLK_4:
                            b++;
                            SDL_Log("%d, %d, %d, %d",a,b,c,d);
                            break;
                        case SDLK_5:
                            c--;
                            SDL_Log("%d, %d, %d, %d",a,b,c,d);
                            break;
                        case SDLK_6:
                            c++;
                            SDL_Log("%d, %d, %d, %d",a,b,c,d);
                            break;
                        case SDLK_7:
                            d--;
                            SDL_Log("%d, %d, %d, %d",a,b,c,d);
                            break;
                        case SDLK_8:
                            d++;
                            SDL_Log("%d, %d, %d, %d",a,b,c,d);
                            break;

                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        //logo 1
        boxRGBA(renderer,0,0,500,500,0,0,0,255);
        //the P
        filledEllipseRGBA(renderer, 270, 138, 74, 65, 0xDF, 0, 0x24,255);
        boxRGBA(renderer, 290, 185, 325, 230,0,0,0,255);
        filledEllipseRGBA(renderer, 313, 140, 30, 49, 0xDF, 0, 0x24,255);
        filledEllipseRGBA(renderer, 307, 179, 22, 11, 0xDF, 0, 0x24,255);
        boxRGBA(renderer, 190, 70, 219, 230, 0,0,0,255);
        //a bi of the s
        Sint16 p2x[4] = {184, 220, 308, 261};
        Sint16 p2y[4] = {268, 278, 256, 243};
        filledPolygonRGBA(renderer, p2x,p2y,4,0,0xAB,0x9F,255);
        Sint16 p3x[4] = {127, 157, 224, 196};
        Sint16 p3y[4] = {251, 260, 230, 220};
        filledPolygonRGBA(renderer, p3x,p3y,4,0,0xAB,0x9F,255);

        //back to the p
        boxRGBA(renderer, 220, 70, 260, 300, 0xDF, 0, 0x24 ,255);
        boxRGBA(renderer, 260, 75, 290, 120, 0xDF, 0, 0x24 ,255);
        boxRGBA(renderer, 290, 95, 325, 185, 0xDF, 0, 0x24 ,255);
        filledTrigonRGBA(renderer, 260, 70, 260, 75, 290,75, 0xDF, 0, 0x24,255);
        filledTrigonRGBA(renderer, 220, 64, 220, 70, 260,70, 0xDF, 0, 0x24,255);
        filledTrigonRGBA(renderer, 220,300, 260,300, 260, 307, 0xDF, 0, 0x24,255);

        filledCircleRGBA(renderer, 275,120,14,0,0,0,255);//background color
        boxRGBA(renderer, 261, 120, 289, 230, 0,0,0,255);

        //the s
        //at this point I was finding it a bit tedious to try and replicate the curves of the warped 3D perspective of the S sooooo...
        filledTrigonRGBA(renderer, 261, 307, 261, 293, 300, 300, 0xF3,0xC3,0,255);
        Sint16 p1x[4] = {262, 301, 408, 368};
        Sint16 p1y[4] = {293, 300, 281, 273};
        filledPolygonRGBA(renderer, p1x,p1y,4,0,0xAB,0x9F,255);
        filledTrigonRGBA(renderer, 196, 220, 219, 229, 219, 210, 0x2E, 0x6D, 0xB4, 255);

        Sint16 p4x[24] = {369, 408, 432, 441, 444, 442, 437, 416, 391, 381, 366, 357, 333, 306, 261, 309, 334, 350, 363, 376, 383, 388, 391, 385};
        Sint16 p4y[24] = {273, 281, 275, 262, 249, 242, 229, 216, 211, 210, 212, 213, 215, 222, 242, 256, 244, 241, 240, 240, 243, 246, 257, 266};
        filledPolygonRGBA(renderer, p4x,p4y,24,0x2E, 0x6D, 0xB4,255);

        Sint16 p5x[] = {219, 201, 190, 180, 161, 145, 131, 121, 114, 115, 118, 127, 157, 154, 156, 162, 169, 176, 183};
        Sint16 p5y[] = {277, 288, 293, 296, 298, 297, 293, 286, 278, 264, 256, 251, 260, 265, 270, 273, 273, 271, 268};
        filledPolygonRGBA(renderer, p5x,p5y,19,0xF3,0xC3,0,255);

        stringRGBA(renderer, 200,400, "Play Station",255,255,255,255);

        //logo 2
        boxRGBA(renderer, 500,0,1000,500, 255,255,255,255);

        SDL_RenderPresent(renderer);
    }

    //clean up zone
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}