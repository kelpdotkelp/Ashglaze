#ifndef VERTEX_H
#define VERTEX_H

#include "modelObject.h"
#include "num/num.h"

namespace geo
{
    class Vertex : public ModelObject
    {
        private:
            num::Vec3 position;

        public:
            Vertex();
            Vertex(float x, float y, float z);

            std::string toString() final;

            num::Vec3 getPos() const;
    };
}

#endif