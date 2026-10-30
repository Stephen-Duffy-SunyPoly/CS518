#include "math.hpp"

SDL_FPoint operator+(const SDL_FPoint & a, const SDL_FPoint & b) {
    SDL_FPoint result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

SDL_FPoint &operator+=(SDL_FPoint & a, const SDL_FPoint & b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

SDL_FPoint operator-(const SDL_FPoint & a, const SDL_FPoint & b) {
    SDL_FPoint result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

SDL_FPoint &operator*=(SDL_FPoint & a, const float b) {
    a.x *= b;
    a.y *= b;
    return a;
}

SDL_FPoint operator*(const SDL_FPoint & a, const float b) {
    SDL_FPoint result;
    result.x = a.x * b;
    result.y = a.y * b;
    return result;
}

float operator*(const SDL_FPoint &a) {
    return a.x*a.x + a.y*a.y;
}