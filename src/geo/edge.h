#ifndef EDGE_H
#define EDGE_H

#include <vector>

#include "modelObject.h"
#include "num/num.h"

namespace geo
{
    class Vertex;
    class Face;

    class Edge : public ModelObject
    {
        private:
            std::vector<Vertex *> vertices;
            std::vector<Face *> faces;

            unsigned int VBOIndex;//Where this edge is in its VBO
        
        public:
            friend class Object3D;
            friend class Vertex;
            friend class Face;

            Edge(Vertex* v1, Vertex* v2);

            std::vector<Vertex*> getVertices();

            num::Vec3 getMidpoint();

            std::string toString();
    };
}

#endif