#ifndef VEC3_H
#define VEC3_H

#include <string>
#include <cmath>

namespace num
{
    class Vec3
    {
        public:
            float x;
            float y;
            float z;

            Vec3(float x, float y, float z);
            Vec3();

            float mag();
            Vec3 normalize();

            Vec3 operator+(Vec3 other);
            Vec3 operator-(Vec3 other);
            Vec3 operator*(float scalar);//Vector-scalar multiplication

            std::string toString();
    };
}

num::Vec3 operator*(float scalar, num::Vec3 other);

#endif