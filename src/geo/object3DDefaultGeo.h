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
        CUBE,
        OCTAHEDRON,
        SPHERE
    };

    class DefaultGeoGenerator
    {
        private:
            static float cubeVertices[];
            static int cubeVerticesIndex[];

            static float octahedronVertices[];
            static int octahedronVerticesIndex[];

            static void createObject3DDataStructure(Object3D* object,
                float* vertexArray, unsigned int vertexArrayLength,
                int* indexArray, unsigned int indexArrayLength);

        public:
            static void generateCube(Object3D* object);
            static void generateOctahedron(Object3D* object);
            static void generateSphere(Object3D* object, int recursionDepth);
    };
}

#endif