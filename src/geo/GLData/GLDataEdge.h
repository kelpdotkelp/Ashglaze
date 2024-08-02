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
            GLDataEdge();
            GLDataEdge(unsigned int vertexLength);

            void generateVBOData(std::list<Edge>* edges);

            virtual void configureGLVertexAttribs();

            void setPos(unsigned int index, num::Vec3 pos, unsigned int vertexIndex);
    };
}

#endif