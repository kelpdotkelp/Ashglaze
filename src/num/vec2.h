#ifndef VEC2_H
#define VEC2_H

#include <string>
#include <cmath>

namespace num
{
    class Vec2
    {
        public:
            float x;
            float y;

            Vec2(float x, float y);
            Vec2();

            float mag();

            Vec2 operator+(Vec2& other);
            Vec2 operator-(Vec2& other);
            Vec2 operator*(float scalar);//Vector-scalar multiplication

            std::string toString();
    };
}

num::Vec2 operator*(float scalar, num::Vec2& other);

#endif