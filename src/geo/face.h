#ifndef FACE_H
#define FACE_H

#include <vector>

#include "edge.h"
#include "modelObject.h"
#include "object3D.h"
#include "vertex.h"
#include "num/num.h"

namespace geo
{
    class Face : public ModelObject
    {
        private:
            std::vector<Vertex*> vertices;
            std::vector<Edge*> edges;

            unsigned int VBOIndex;//Where this face is in its VBO

        public:
            friend class Object3D;
            friend class Vertex;
            friend class Edge;

            Face(Vertex* v1, Vertex* v2, Vertex* v3);

            std::vector<Vertex*> getVertices();
            std::vector<Edge*> getEdges();

            void addEdge(Edge* edge);

            num::Vec3 getNormal();

            std::string toString();
    };
}

#endif