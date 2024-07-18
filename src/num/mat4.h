#ifndef MAT4_H
#define MAT4_H

#include <string>
#include <algorithm>

namespace num
{
    class Mat4
    {
        private:
            const static float identity[4][4];

        public:
            float nums[4][4];

            Mat4();//Creates a new identity matrix

            Mat4 operator+(Mat4& other);
            Mat4 operator-(Mat4& other);
            Mat4 operator*(float scalar);//Matrix-scalar multiplication
            Mat4 operator*(Mat4& other);

            std::string toString();
    };
}

num::Mat4 operator*(float scalar, num::Mat4& other);

#endif