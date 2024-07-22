#ifndef EDGE_H
#define EDGE_H

#include <vector>

#include "modelObject.h"
#include "vertex.h"

namespace geo
{
    class Edge : public ModelObject
    {
        private:
            std::vector<Vertex *> vertices;
        
        public:
            Edge(Vertex* v1, Vertex* v2);

            std::vector<Vertex*> getVertices();

            std::string toString();
    };
}

#endif