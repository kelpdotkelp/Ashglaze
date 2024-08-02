#ifndef EDGE_H
#define EDGE_H

#include <unordered_set>
#include <iterator>

#include "num/num.h"
#include "modelObject.h"

namespace geo
{
    class Vertex;
    class Face;
    class Object3D;

    class Edge : public ModelObject
    {
        private:
            std::unordered_set<Vertex *> vertices;
            std::unordered_set<Face *> faces;

            unsigned int VBOIndex;//Where this edge is in its VBO
        
        public:
            friend class Object3D;
            friend class Vertex;
            friend class Face;

            Edge(Vertex* v1, Vertex* v2);
            ~Edge();

            std::unordered_set<Vertex*> getVertices();

            Vertex* vertex0();
            Vertex* vertex1();

            num::Vec3 getMidpoint();
            unsigned int getVBOIndex();

            void setVBOIndex(unsigned int VBOIndex);

            std::string toString();
    };
}

#endif