#ifndef SPRITES_SPRITESHEET_HPP
#define SPRITES_SPRITESHEET_HPP
#include "SDL3/SDL_render.h"

namespace qnd {
    class SpriteSheet {
        SDL_Texture* texture;
        int sprite_width;
        int sprite_height;
        int cols;

    public:
        SpriteSheet(SDL_Texture* texture, int cols, int rows);

        SDL_FRect operator[](int index) const;
        virtual void draw(SDL_Renderer* renderer, int index, int x, int y, float scale);
        virtual void draw(SDL_Renderer* renderer, int index, SDL_FPoint pos, float scale);

        [[nodiscard]] int getWidth() const;
        [[nodiscard]] int getHeight() const;
        [[nodiscard]] SDL_Texture* getTexture() const;

        virtual ~SpriteSheet() = default;
    };

    class Font: public SpriteSheet {
    public:
        explicit Font(SDL_Texture* texture);

        int textWidth(const char* text) const;
        int textHeight(const char* text) const;

        void drawString(SDL_Renderer * renderer, const char * str, int x, int y);
        void drawStringCentered(SDL_Renderer * renderer, const char * str, int x, int y);

    };
}
#endif //SPRITES_SPRITESHEET_HPP
