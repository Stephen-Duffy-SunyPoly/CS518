//quick and dirty math functions and operator overloads
#ifndef QND_MATH_HPP
#define QND_MATH_HPP
#include "SDL3/SDL_rect.h"
SDL_FPoint operator+(const SDL_FPoint & a, const SDL_FPoint & b);

SDL_FPoint &operator+=(SDL_FPoint & a, const SDL_FPoint & b);

SDL_FPoint operator-(const SDL_FPoint & a, const SDL_FPoint & b);

SDL_FPoint &operator*=(SDL_FPoint & a, const float b);

SDL_FPoint operator*(const SDL_FPoint & a, const float b);

float operator*(const SDL_FPoint &a);
#endif //QND_MATH_HPP
