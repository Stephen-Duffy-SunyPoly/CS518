#ifndef SPRITES_SPRITESHEET_HPP
#define SPRITES_SPRITESHEET_HPP
#include "SDL3/SDL_render.h"

class SpriteSheet {
    SDL_Texture* texture;
    int sprite_width;
    int sprite_height;
    int cols;

public:
    SpriteSheet(SDL_Texture* texture, int cols, int rows);

    SDL_FRect operator[](int index) const;
    virtual void draw(SDL_Renderer* renderer, int index, int x, int y, float scale);

    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;
    [[nodiscard]] SDL_Texture* getTexture() const;

    virtual ~SpriteSheet() = default;
};

#endif //SPRITES_SPRITESHEET_HPP
