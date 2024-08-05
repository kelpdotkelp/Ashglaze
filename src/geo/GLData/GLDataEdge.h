#ifndef GL_DATA_EDGE_H
#define GL_DATA_EDGE_H

#include "GLData.h"
#include "../edge.h"
#include "../vertex.h"
#include "num/num.h"

namespace geo
{
    class GLDataEdge: public GLData
    {
        public:
            static unsigned int vertexLength;

            GLDataEdge();

            void generateVBOData(std::list<Edge>* edges);

            virtual void configureGLVertexAttribs();

            void setPos(unsigned int index, num::Vec3 pos, unsigned int vertexIndex);
    };
}

#endif