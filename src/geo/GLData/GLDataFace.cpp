#include "GLDataFace.h"
#include "../face.h"
#include "../vertex.h"

namespace geo
{
    GLDataFace::GLDataFace() : GLData() {}

    GLDataFace::GLDataFace(unsigned int vertexLength) :
        GLData(vertexLength, GL_TRIANGLES) {}

    void GLDataFace::generateVBOData(std::list<Face>* faces)
    {
        vertexCount = 3*(*faces).size();

        VBOData.clear();//Remove list contents

        unsigned int VBOIndex = 0;
        num::Vec3 pos;
        num::Vec3 normal;
        for (auto& face: *faces)
        {
            for (int i=0; i<3; i++)
            {
                VBOData.push_back((float)face.getID());
                pos = face.getWindingOrder()[i]->getPos();
                VBOData.push_back(pos.x);
                VBOData.push_back(pos.y);
                VBOData.push_back(pos.z);
                normal = face.getNormal();
                VBOData.push_back(normal.x);
                VBOData.push_back(normal.y);
                VBOData.push_back(normal.z);
            }

            face.setVBOIndex(3*vertexLength*VBOIndex);
            VBOIndex++;
        }
    }

    void GLDataFace::configureGLVertexAttribs()
    {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //ID
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, vertexLength*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexLength*sizeof(float), (void*)(sizeof(float)));
        glEnableVertexAttribArray(1);
        //Normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertexLength*sizeof(float), (void*)(4*sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    void GLDataFace::setPos(unsigned int index, num::Vec3 pos, unsigned int vertexIndex)
    {
        VBOData[index+1 + vertexIndex*vertexLength] = pos.x;
        VBOData[index+2 + vertexIndex*vertexLength] = pos.y;
        VBOData[index+3 + vertexIndex*vertexLength] = pos.z;
    }

    void GLDataFace::setNormal(unsigned int index, num::Vec3 normal)
    {
        for (int i=0; i<3; i++)
        {
            VBOData[index+4 + i*vertexLength] = normal.x;
            VBOData[index+5 + i*vertexLength] = normal.y;
            VBOData[index+6 + i*vertexLength] = normal.z;
        }
    }
}