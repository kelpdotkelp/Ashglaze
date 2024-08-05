#ifndef OBJECT_3D_DEFAULT_GEO
#define OBJECT_3D_DEFAULT_GEO

#include <list>

namespace geo
{
    class Vertex;
    class Edge;
    class Face;
    class Object3D;

    enum class BasePrimitives
    {
        CUBE
    };

    class DefaultGeoGenerator
    {
        private:
            static float cubeVertices[];
            static int cubeVerticesIndex[];

        public:
            static void generateCube(Object3D* object);
    };
}

#endif