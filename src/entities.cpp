#include "entities.hpp"

Vec2 Vec2::operator+(const Vec2& other) const
{
    return {x_ + other.x_, y_ + other.y_};
}

Vec2 Vec2::operator*(const double val) const
{
    return {(float)(x_ * val), (float)(y_ * val)};
}
