#include "entities.hpp"

Vec2 Vec2::operator+(const Vec2& other) const
{
    return {x_ + other.x_, y_ + other.y_};
}

Vec2 Vec2::operator*(const double val) const
{
    return {(float)(x_ * val), (float)(y_ * val)};
}


void Snake::Update(int window_width, int window_height, double delta_time)
{
    
    Vec2 movement = velocity_ * delta_time;
    rect_.x = rect_.x + movement.x_;
    rect_.y = rect_.y + movement.y_;
    if(rect_.x < -rect_.w) rect_.x = window_width;
    if(rect_.x > window_width) rect_.x = 0;
    if(rect_.y < -rect_.h) rect_.y = window_height;
    if(rect_.y > window_height) rect_.y = 0;
}