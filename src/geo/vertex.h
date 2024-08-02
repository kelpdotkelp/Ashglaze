#ifndef VERTEX_H
#define VERTEX_H

#include <unordered_set>

#include "modelObject.h"
#include "num/num.h"

namespace geo
{
    class Edge;
    class Face;

    class Vertex : public ModelObject
    {
        private:
            num::Vec3 position;
            std::unordered_set<Edge*> edges;
            std::unordered_set<Face*> faces;

            unsigned int VBOIndex;//Where this vertex is in its VBO

        public:
            friend class Object3D;
            friend class Edge;
            friend class Face;

            Vertex();
            Vertex(float x, float y, float z);

            std::string toString() final;

            num::Vec3 getPos() const;
            unsigned int getVBOIndex();

            void setVBOIndex(unsigned int VBOIndex);
    };
}

#endif