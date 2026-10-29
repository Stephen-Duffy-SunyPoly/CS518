#include "spriteSheet.hpp"

qnd::SpriteSheet::SpriteSheet(SDL_Texture* texture, const int cols, const int rows) {
    this->texture = texture;
    this->cols = cols;
    sprite_width = texture->w / cols;
    sprite_height = texture->h / rows;
}
SDL_FRect qnd::SpriteSheet::operator[](const int index) const {
    return {
        .x = static_cast<float>((index % cols) * sprite_width),
        .y = static_cast<float>((index / cols) * sprite_height), // NOLINT(*-integer-division)
        .w = static_cast<float>(sprite_width),
        .h = static_cast<float>(sprite_height)
    };
}
void qnd::SpriteSheet::draw(SDL_Renderer* renderer, const int index, const int x, const int y, const float scale) {
    draw(renderer, index, {static_cast<float>(x),static_cast<float>(y)},scale);
}

void qnd::SpriteSheet::draw(SDL_Renderer *renderer, int index, SDL_FPoint pos, float scale) {
    const SDL_FRect src = this->operator[](index);
    const SDL_FRect dst = {
        .x = pos.x,
        .y = pos.y,
        .w = static_cast<float>(sprite_width)*scale,
        .h = static_cast<float>(sprite_height)*scale
    };
    SDL_RenderTexture(renderer, texture, &src,&dst);
}

int qnd::SpriteSheet::getWidth() const {
    return  sprite_width;
}
int qnd::SpriteSheet::getHeight() const {
    return  sprite_height;
}

SDL_Texture* qnd::SpriteSheet::getTexture() const {
    return texture;
}

qnd::Font::Font(SDL_Texture* texture): SpriteSheet(texture, 16,8){}
int qnd::Font::textWidth(const char* text) const {
    //TODO handle new lines
    return  static_cast<int>(SDL_strlen(text)) * getWidth();
}
int qnd::Font::textHeight([[maybe_unused]] const char* text) const {
    //TODO handle new lines
    return getHeight();
}

void qnd::Font::drawString(SDL_Renderer* renderer, const char* str, const int x, const int y) {
    int i =0;
    while (*str != '\0') {
        draw(renderer, *str, x+i*getWidth(),y,1.0f);
        str++;
        i++;
    }
}

void qnd::Font::drawStringCentered(SDL_Renderer* renderer, const char* str, const int x, const int y) {
    const int stringWidth = textWidth(str);
    const int stringHeight = textHeight(str);
    drawString(renderer, str, x-stringWidth/2,y-stringHeight/2);
}
