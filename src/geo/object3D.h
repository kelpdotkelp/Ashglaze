#ifndef PRIMITIVE_3D_H
#define PRIMITIVE_3D_H

#include <glad/glad.h>

#include <list>
#include <vector>
#include <unordered_map>

#include "edge.h"
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

    class Object3D : public ModelObject
    {
        private:
            //Store basic vertex and index information for
            //Some fundamental primitives.
            const static float cubeVertexPosData[3*8];
            const static int cubeEBOFaceData[6*6];//Two triangles (6 vertices) per face (6 faces)
            const static int cubeEBOEdgeData[2*18];//18 edges, 2 vertices per edge

            unsigned int        meshVBO;
            unsigned int        meshVAO;
            std::vector<float>  meshVBOData;
            //1 ID, 3 position, 3 normal = 7
            unsigned int meshVBOStride = 7;

            unsigned int        verticesVBO;
            unsigned int        verticesVAO;
            std::vector<float>  verticesVBOData;
            //1 ID, 3 position = 4
            unsigned int verticesVBOStride = 4;

            unsigned int        edgesVBO;
            unsigned int        edgesVAO;
            std::vector<float>  edgesVBOData;
            //1 ID, 3 position = 4
            unsigned int edgesVBOStride = 4;

            //Geometry data
            std::list<Vertex> vertices;
            std::list<Face>   faces;
            std::list<Edge>   edges;

            void translateVertex(unsigned int vertexID, num::Vec3 transAmount);

            void generateVerticesVBOData();
            void generateEdgesVBOData();
            void generateFacesVBOData();
            void sendVBOToGPU();

        public:
            Object3D(BasePrimitives type);
            Object3D();

            void renderMesh();
            void renderVertices();
            void renderEdges();

            void translateGeoFeature(unsigned int geoID, num::Vec3 transAmount);
            void insertVertex(unsigned int geoID);

            std::string toString();
    };
}

#endif