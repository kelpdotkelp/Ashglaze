#ifndef GL_DATA_VERTEX_H
#define GL_DATA_VERTEX_H

#include "GLData.h"
#include "num/num.h"

namespace geo
{
    class Vertex;

    class GLDataVertex : public GLData
    {
        public:
            GLDataVertex();
            GLDataVertex(unsigned int vertexLength);

            void generateVBOData(std::list<Vertex>* vertices);

            virtual void configureGLVertexAttribs();

            void setPos(unsigned int index, num::Vec3 pos);
    };
}

#endif