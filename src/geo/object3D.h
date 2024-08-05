#ifndef PRIMITIVE_3D_H
#define PRIMITIVE_3D_H

#include <glad/glad.h>

#include <list>
#include <vector>
#include <utility>
#include <unordered_map>

#include "edge.h"
#include "face.h"
#include "shader.h"
#include "shaderProgram.h"
#include "vertex.h"
#include "GLData/GLDataVertex.h"
#include "GLData/GLDataEdge.h"
#include "GLData/GLDataFace.h"
#include "object3DDefaultGeo.h"

namespace geo
{
    class Object3D : public ModelObject
    {
        friend class DefaultGeoGenerator;

        private:
            GLDataVertex verticesGL;
            GLDataEdge   edgesGL;
            GLDataFace   facesGL;

            //Geometry data
            std::list<Vertex> vertices;
            std::list<Face>   faces;
            std::list<Edge>   edges;

            void translateVertex(Vertex* vertex, num::Vec3 transAmount);

            Vertex* addVertexObject(num::Vec3 position);
            Edge* addEdgeObject(Vertex* v1, Vertex* v2);
            Face* addFaceObject(Vertex* v1, Vertex* v2, Vertex* v3,
                Edge* e1 = nullptr, Edge* e2 = nullptr, Edge* e3 = nullptr);

            void deleteEdgeObject(Edge* edge);
            void deleteFaceObject(Face* face);

            Edge* edgeExists(Vertex* v1, Vertex* v2);
            void associateEdgeWithFace(Edge* edge, Face* face);
            void generateAndSendVBOsToGPU();
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