#ifndef FACE_H
#define FACE_H

#include <vector>

#include "edge.h"
#include "modelObject.h"
#include "vertex.h"
#include "num/num.h"

namespace geo
{
    class Face : public ModelObject
    {
        private:
            std::vector<Vertex*> vertices;
            std::vector<Edge*> edges;

        public:
            Face(Vertex* v1, Vertex* v2, Vertex* v3);

            std::vector<Vertex*> getVertices();
            std::vector<Edge*> getEdges();

            void addEdge(Edge* edge);

            num::Vec3 getNormal();

            std::string toString();
    };
}

#endif