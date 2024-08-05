#include "GLDataEdge.h"

namespace geo
{
    unsigned int GLDataEdge::vertexLength = 4;

    GLDataEdge::GLDataEdge() 
        : GLData(GLDataEdge::vertexLength, GL_LINES) {}

    void GLDataEdge::generateVBOData(std::list<Edge>* edges)
    {
        vertexCount = 2*(*edges).size();

        VBOData.clear();//Remove list contents

        unsigned int VBOIndex = 0;
        num::Vec3 pos;
        for (auto& edge: *edges)
        {
            VBOData.push_back((float)edge.getID());
            pos = edge.vertex0()->getPos();
            VBOData.push_back(pos.x);
            VBOData.push_back(pos.y);
            VBOData.push_back(pos.z);

            VBOData.push_back((float)edge.getID());
            pos = edge.vertex1()->getPos();
            VBOData.push_back(pos.x);
            VBOData.push_back(pos.y);
            VBOData.push_back(pos.z);

            edge.setVBOIndex(2*vertexLength*VBOIndex);
            VBOIndex++;
        }
    }

    void GLDataEdge::configureGLVertexAttribs()
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

    void GLDataEdge::setPos(unsigned int index, num::Vec3 pos, unsigned int vertexIndex)
    {
        VBOData[index+1 + vertexIndex*vertexLength] = pos.x;
        VBOData[index+2 + vertexIndex*vertexLength] = pos.y;
        VBOData[index+3 + vertexIndex*vertexLength] = pos.z;
    }
}