#include "mat4.h"

namespace num
{
//Column major
const float Mat4::identity[4][4] = {
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f}
};

Mat4::Mat4()
{
    std::copy(&identity[0][0], &identity[0][0]+16, &nums[0][0]);
}

Mat4 Mat4::operator+(Mat4& other)
{
    Mat4 out;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            out.nums[i][j] = nums[i][j] + other.nums[i][j];
        }
    }
    return out;
}

Mat4 Mat4::operator-(Mat4& other)
{
    Mat4 out;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            out.nums[i][j] = nums[i][j] - other.nums[i][j];
        }
    }
    return out;
}

Mat4 Mat4::operator*(float scalar)
{
    Mat4 out;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            out.nums[i][j] = scalar*nums[i][j];
        }
    }
    return out;
}

Mat4 Mat4::operator*(Mat4& other)
{
    Mat4 out;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            out.nums[i][j] = nums[i][0]*other.nums[0][j] +
                nums[i][1]*other.nums[1][j] +
                nums[i][2]*other.nums[2][j] +
                nums[i][3]*other.nums[3][j];
        }
    }
    return out;
}

std::string Mat4::toString()
{
    std::string out = "";
    for(int i=0; i<4; i++)
    {
        out += "[";
        for(int j=0; j<4; j++)
        {
            if(j != 3)
                out += std::to_string(nums[i][j]) + " ";
            else
                out += std::to_string(nums[i][j]);
        }
        out += "]\n";
    }

    return out;
}
}

num::Mat4 operator*(float scalar, num::Mat4& other)
{
    return other * scalar;
}