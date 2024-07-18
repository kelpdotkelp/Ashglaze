#include "vec3.h"

namespace num
{
    Vec3::Vec3(float x, float y, float z)
    {
        Vec3::x = x;
        Vec3::y = y;
        Vec3::z = z;
    }

    Vec3::Vec3()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

    float Vec3::mag() {return std::sqrt(x*x + y*y + z*z);}

    Vec3 Vec3::normalize()
    {
        return Vec3(x/mag(), y/mag(), z/mag());
    }

    Vec3 Vec3::operator+(Vec3 other)
    {
        return Vec3(x+other.x, y+other.y, z+other.z);
    }

    Vec3 Vec3::operator-(Vec3 other)
    {
        return Vec3(x-other.x, y-other.y, z-other.z);
    }

    Vec3 Vec3::operator*(float scalar)
    {
        return Vec3(scalar*x, scalar*y, scalar*z);
    }

    std::string Vec3::toString()
    {
        std::string str = "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
        return str;
    }
}

num::Vec3 operator*(float scalar, num::Vec3 other)
{
    return other * scalar;
}