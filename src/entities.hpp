#include <SDL3/SDL.h>
struct Vec2
{
    float x_;
    float y_;
    
    Vec2 operator+(const Vec2& other) const;
    Vec2 operator*(const double val) const;
};
struct Snake
{
    SDL_FRect rect_;
    Vec2 velocity_;
    void Update(int window_width, int window_height, double delta_time);
};
struct Apple
{
    SDL_FRect rect_;
};