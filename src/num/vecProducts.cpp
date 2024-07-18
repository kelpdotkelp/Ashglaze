#include "vecProducts.h"

namespace num
{
    Vec3 crossProduct(Vec3 v, Vec3 w)
    {
        return Vec3(
            (v.y * w.z) - (v.z * w.y),
            (v.z * w.x) - (v.x * w.z),
            (v.x * w.y) - (v.y * w.x)
        );
    }
}