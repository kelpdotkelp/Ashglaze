#ifndef PRIMITIVE_3D_H
#define PRIMITIVE_3D_H

#include <glad/glad.h>

#include <list>
#include <vector>
#include <unordered_map>

#include "face.h"
#include "shader.h"
#include "shaderProgram.h"
#include "vertex.h"

namespace geo
{
    enum class BasePrimitives
    {
        CUBE
    };

    class Primitive3D
    {
        private:
            //Store basic vertex and index information for
            //Some fundamental primitives.
            const static float cubeVertexPosData[3*8];
            const static int cubeEBOData[6*6];//Two triangles (6 vertices) per face (6 faces)

            unsigned int        meshVBO;
            unsigned int        meshVAO;
            std::vector<float>  meshVBOData;

            unsigned int        verticesVBO;
            unsigned int        verticesVAO;
            std::vector<float>  verticesVBOData;

            //Geometry data
            std::list<Vertex> vertices;
            std::list<Face>   faces;

        public:
            Primitive3D(BasePrimitives type);
            Primitive3D();

            void renderMesh();
            void renderVertices();
    };
}

#endif