#ifndef FACE_H
#define FACE_H

#include <vector>

#include "modelObject.h"
#include "vertex.h"
#include "num/num.h"

namespace geo
{
    class Face : public ModelObject
    {
        private:
            std::vector<Vertex*> vertices;

        public:
            Face(Vertex* v1, Vertex* v2, Vertex* v3);

            std::vector<Vertex*> getVertices();

            num::Vec3 getNormal();

            std::string toString();
    };
}

#endif