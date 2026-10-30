//quick and dirty math functions and operator overloads
#ifndef QND_MATH_HPP
#define QND_MATH_HPP
#include "SDL3/SDL_rect.h"
static SDL_FPoint operator+(const SDL_FPoint & a, const SDL_FPoint & b);

static SDL_FPoint &operator+=(SDL_FPoint & a, const SDL_FPoint & b);

static SDL_FPoint operator-(const SDL_FPoint & a, const SDL_FPoint & b);

static SDL_FPoint &operator*=(SDL_FPoint & a, const float b);

static SDL_FPoint operator*(const SDL_FPoint & a, const float b);

static float operator*(const SDL_FPoint &a);
#endif //QND_MATH_HPP
