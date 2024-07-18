#ifndef MAT4_TRANSFORM_H
#define MAT4_TRANSFORM_H

#include <cmath>

#include "mat4.h"
#include "vec3.h"
#include "vecProducts.h"

namespace num
{
    Mat4 translate(Mat4 applyTo, float x, float y, float z);
    Mat4 translate(Mat4 applyTo, Vec3 vec);

    Mat4 scale(Mat4 applyTo, float scaleX, float scaleY, float scaleZ);

    Mat4 rotateX(Mat4 applyTo, float angleDegree);
    Mat4 rotateY(Mat4 applyTo, float angleDegree);
    Mat4 rotateZ(Mat4 applyTo, float angleDegree);

    Mat4 perspective(float aspectRatio, float fovDegree, float near, float far);

    Mat4 lookAt(Vec3 up, Vec3 direction, Vec3 right, Vec3 position);
}

#endif