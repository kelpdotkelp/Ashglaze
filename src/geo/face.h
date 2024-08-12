#ifndef FACE_H
#define FACE_H

#include <unordered_set>
#include <vector>

#include "modelObject.h"
#include "num/num.h"


namespace geo
{
        
    class Edge;
    class Vertex;

    class Face : public ModelObject
    {
        private:
            std::unordered_set<Vertex*> vertices;
            std::unordered_set<Edge*> edges;
            std::vector<Vertex*> windingOrder;

            unsigned int VBOIndex;//Where this face is in its VBO

        public:
            friend class Object3D;
            friend class Vertex;
            friend class Edge;

            Face(Vertex* v1, Vertex* v2, Vertex* v3);
            Face(Vertex* v1, Vertex* v2, Vertex* v3, Edge* e1, Edge* e2, Edge* e3);
            ~Face();

            std::vector<Vertex*> getWindingOrder();

            Edge* edge0();
            Edge* edge1();
            Edge* edge2();

            num::Vec3 getNormal();
            Vertex* getThirdVertex(Vertex* vertex0ID, Vertex* vertex1ID);
            num::Vec3 getCentroid();
            unsigned int getVBOIndex();

            void setVBOIndex(unsigned int VBOIndex);

            std::string toString();
    };
}

#endif