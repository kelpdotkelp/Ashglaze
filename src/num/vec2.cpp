#include "vec2.h"

namespace num
{
    Vec2::Vec2()
    {
        
    }

    Vec2::Vec2(float x, float y)
    {
        Vec2::x = x;
        Vec2::y = y;
    }

    float Vec2::mag() {return sqrt(x*x + y*y);}

    Vec2 Vec2::operator+(Vec2& other)
    {
        return Vec2(x+other.x, y+other.y);
    }

    Vec2 Vec2::operator-(Vec2& other)
    {
        return Vec2(x-other.x, y-other.y);
    }

    Vec2 Vec2::operator*(float scalar)
    {
        return Vec2(scalar*x, scalar*y);
    }

    std::string Vec2::toString()
    {
        std::string str = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        return str;
    }
}

num::Vec2 operator*(float scalar, num::Vec2& other)
{
    return other * scalar;
}
