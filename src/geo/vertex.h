#ifndef VERTEX_H
#define VERTEX_H

#include <list>

#include "modelObject.h"
#include "object3D.h"
#include "num/num.h"

namespace geo
{
    class Face;

    class Vertex : public ModelObject
    {
        private:
            num::Vec3 position;
            std::list<Edge*> edges;
            std::list<Face*> faces;

            unsigned int VBOIndex;//Where this vertex is in its VBO

        public:
            friend class Object3D;
            friend class Edge;
            friend class Face;

            Vertex();
            Vertex(float x, float y, float z);

            std::string toString() final;

            num::Vec3 getPos() const;
    };
}

#endif