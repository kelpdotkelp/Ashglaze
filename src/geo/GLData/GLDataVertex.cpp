#include "GLDataVertex.h"
#include "../vertex.h"

namespace geo
{
    GLDataVertex::GLDataVertex() : GLData() {}

    GLDataVertex::GLDataVertex(unsigned int vertexLength) :
        GLData(vertexLength, GL_POINTS) {}

    void GLDataVertex::configureGLVertexAttribs()
    {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //ID
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, vertexLength*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexLength*sizeof(float), (void*)(sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void GLDataVertex::generateVBOData(std::list<Vertex>* vertices)
    {
        vertexCount = (*vertices).size();

        VBOData.clear();

        unsigned int VBOIndex = 0;
        num::Vec3 pos;
        for (auto& vertex: (*vertices))
        {
            VBOData.push_back((float)vertex.getID());
            pos = vertex.getPos();
            VBOData.push_back(pos.x);
            VBOData.push_back(pos.y);
            VBOData.push_back(pos.z);

            vertex.setVBOIndex(vertexLength*VBOIndex);
            VBOIndex++;
        }
    }

    void GLDataVertex::setPos(unsigned int index, num::Vec3 pos)
    {
        VBOData[index+1] = pos.x;
        VBOData[index+2] = pos.y;
        VBOData[index+3] = pos.z;
    }
}