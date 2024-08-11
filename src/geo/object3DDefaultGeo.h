#ifndef OBJECT_3D_DEFAULT_GEO
#define OBJECT_3D_DEFAULT_GEO

#include <list>
#include <vector>

#include "num/num.h"

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

            static void createFace(Object3D* object, Vertex* v1, Vertex* v2, Vertex* v3);

            static void sphereGenerateVertices(Object3D* object,
                std::vector<std::vector<Vertex*>>* vertexLayerLists, num::Vec3 startingVertex,
                num::Vec3 sideInc, int layerCount, int offset=0);

            static void sphereMeshHalf(Object3D* object,
                std::vector<std::vector<Vertex*>>* vertexLayerLists, int layerCount,
                bool flipNormals=false);

        public:
            static void generateCube(Object3D* object);
            static void generateOctahedron(Object3D* object);
            static void generateSphere(Object3D* object, int recursionDepth);
    };
}

#endif