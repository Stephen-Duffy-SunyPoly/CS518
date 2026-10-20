#include "spriteSheet.hpp"
SpriteSheet::SpriteSheet(SDL_Texture* texture, const int cols, const int rows) {
    this->texture = texture;
    this->cols = cols;
    sprite_width = texture->w / cols;
    sprite_height = texture->h / rows;
}
SDL_FRect SpriteSheet::operator[](const int index) const {
    return {
        .x = static_cast<float>((index % cols) * sprite_width),
        .y = static_cast<float>((index / cols) * sprite_height), // NOLINT(*-integer-division)
        .w = static_cast<float>(sprite_width),
        .h = static_cast<float>(sprite_height)
    };
}
void SpriteSheet::draw(SDL_Renderer* renderer, const int index, const int x, const int y, const float scale) {
    const SDL_FRect src = this->operator[](index);
    const SDL_FRect dst = {
        .x = static_cast<float>(x),
        .y = static_cast<float>(y),
        .w = static_cast<float>(sprite_width)*scale,
        .h = static_cast<float>(sprite_height)*scale
    };
    SDL_RenderTexture(renderer, texture, &src,&dst);
}

int SpriteSheet::getWidth() const {
    return  sprite_width;
}
int SpriteSheet::getHeight() const {
    return  sprite_height;
}

SDL_Texture* SpriteSheet::getTexture() const {
    return texture;
}
