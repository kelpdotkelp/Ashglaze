#include "GLData.h"

namespace geo
{
    GLData::GLData()
    {

    }

    GLData::GLData(unsigned int vertexLength, unsigned int geoType)
    {
        this->vertexLength = vertexLength;
        this->geoType = geoType;
    }

    void GLData::generateGLObjects()
    {
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
    }

    void GLData::sendDataToGPU()
    {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, VBOData.size()*sizeof(float), VBOData.data(), GL_DYNAMIC_DRAW);
    }

    void GLData::render()
    {
        glBindVertexArray(VAO);
        glDrawArrays(geoType, 0, vertexCount);
    }
}