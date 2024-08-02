#ifndef GL_DATA_FACE_H
#define GL_DATA_FACE_H

#include "GLData.h"
#include "num/num.h"

namespace geo
{
    class Face;

    class GLDataFace: public GLData
    {
        public:
            GLDataFace();
            GLDataFace(unsigned int vertexLength);

            void generateVBOData(std::list<Face>* faces);

            virtual void configureGLVertexAttribs();

            void setPos(unsigned int index, num::Vec3 pos, unsigned int vertexIndex);
            void setNormal(unsigned int index, num::Vec3 normal);
    };
}

#endif