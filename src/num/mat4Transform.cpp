#include "mat4Transform.h"

namespace num
{
    Mat4 translate(Mat4 applyTo, float x, float y, float z)
    {
        applyTo.nums[0][3] = applyTo.nums[0][3] + x;
        applyTo.nums[1][3] = applyTo.nums[1][3] + y;
        applyTo.nums[2][3] = applyTo.nums[2][3] + z;
        return applyTo;
    }

    Mat4 translate(Mat4 applyTo, Vec3 vec)
    {
        applyTo.nums[0][3] = applyTo.nums[0][3] + vec.x;
        applyTo.nums[1][3] = applyTo.nums[1][3] + vec.y;
        applyTo.nums[2][3] = applyTo.nums[2][3] + vec.z;
        return applyTo;
    }

    Mat4 scale(Mat4 applyTo, float scaleX, float scaleY, float scaleZ)
    {
        applyTo.nums[0][0] = scaleX * applyTo.nums[0][0];
        applyTo.nums[1][1] = scaleY * applyTo.nums[1][1];
        applyTo.nums[2][2] = scaleZ * applyTo.nums[2][2];
        return applyTo;
    }

    Mat4 rotateX(Mat4 applyTo, float angleDegree)
    {
        float rad = (2*acos(0.0) * angleDegree)/180.0f;
        Mat4 rot = Mat4();
        rot.nums[1][1] = cos(rad);
        rot.nums[1][2] = -sin(rad);
        rot.nums[2][1] = sin(rad);
        rot.nums[2][2] = cos(rad);
        
        return rot * applyTo;
    }

    Mat4 rotateY(Mat4 applyTo, float angleDegree)
    {
        float rad = (2*acos(0.0) * angleDegree)/180.0f;
        Mat4 rot = Mat4();
        rot.nums[0][0] = cos(rad);
        rot.nums[0][2] = sin(rad);
        rot.nums[2][0] = -sin(rad);
        rot.nums[2][2] = cos(rad);

        return rot * applyTo;
    }

    Mat4 rotateZ(Mat4 applyTo, float angleDegree)
    {
        float rad = (2*acos(0.0) * angleDegree)/180.0f;
        Mat4 rot = Mat4();
        rot.nums[0][0] = cos(rad);
        rot.nums[0][1] = -sin(rad);
        rot.nums[1][0] = sin(rad);
        rot.nums[1][1] = cos(rad);

        return rot * applyTo;
    }

    Mat4 perspective(float aspectRatio, float fovDegree, float near, float far)
    {
        float fovRad = (2*acos(0.0) * fovDegree)/180.0f;
        Mat4 per = Mat4();
        per.nums[0][0] = 1.0f/(aspectRatio*tan(fovRad*0.5f));
        per.nums[1][1] = 1.0f/tan(fovRad*0.5f);
        per.nums[2][2] = -(far + near) / (far - near);
        per.nums[3][2] = -1;
        per.nums[2][3] = (-2.0f*far*near) / (far - near);
        return per;
    }

    Mat4 lookAt(Vec3 up, Vec3 direction, Vec3 right, Vec3 position)
    {
        Mat4 m1 = Mat4();
        m1.nums[0][0] = right.x;
        m1.nums[0][1] = right.y;
        m1.nums[0][2] = right.z;
        m1.nums[1][0] = up.x;
        m1.nums[1][1] = up.y;
        m1.nums[1][2] = up.z;
        m1.nums[2][0] = direction.x;
        m1.nums[2][1] = direction.y;
        m1.nums[2][2] = direction.z;
        Mat4 m2 = Mat4();
        m2.nums[0][3] = -position.x;
        m2.nums[1][3] = -position.y;
        m2.nums[2][3] = -position.z;
        return m1 * m2;
    }
}