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

bool operator^(const SDL_FRect& r, const SDL_FPoint& p) {
    return p.x >= r.x && p.x <= r.x+r.w && p.y >= r.y && p.y <= r.y+r.h;
}

float operator*(const SDL_FPoint &a) {
    return a.x*a.x + a.y*a.y;
}
