#ifndef GL_DATA_H
#define GL_DATA_H

#include <glad/glad.h>

#include <list>
#include <vector>

namespace geo
{
    class GLData
    {
        protected:
            unsigned int VBO;
            unsigned int VAO;
            std::vector<float> VBOData;
            unsigned int vertexLength;
            unsigned int vertexCount;//Edges have 2 vertices, faces have 3
            unsigned int geoType;
        
        public:
            GLData();
            GLData(unsigned int vertexLength, unsigned int geoType);

            void generateGLObjects();

            void sendDataToGPU();

            void render();

            virtual void configureGLVertexAttribs() = 0;
    };   
}

#endif