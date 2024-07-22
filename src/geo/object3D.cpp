#include "object3D.h"

namespace geo
{
    Object3D::Object3D()
    {

    }

    Object3D::Object3D(BasePrimitives type)
    {
        glGenBuffers(1, &meshVBO);
        glGenVertexArrays(1, &meshVAO);

        glGenBuffers(1, &verticesVBO);
        glGenVertexArrays(1, &verticesVAO);

        glGenBuffers(1, &edgesVBO);
        glGenVertexArrays(1, &edgesVAO);

        if (type == BasePrimitives::CUBE)
        {
            //This is a mapping so the EBO array maps to created vertex IDs
            //It is only used to associate the new vertices with faces.
            std::unordered_map<int, int> tempVertexMap;

            //Generate vertices
            for (int i=0; i<8; i++)
            {
                //Insert a new vertex into the list
                vertices.emplace_back(
                    cubeVertexPosData[i*3],
                    cubeVertexPosData[i*3 + 1],
                    cubeVertexPosData[i*3 + 2]);
                tempVertexMap.insert({i, vertices.back().getID()});
            }

            //Generate faces and associate them with correct vertices
            for (int i=0; i<12; i++)//6 faces, 2 triangles per face
            {
                //Retrieve EBO index, map that index to masterObjectID, retrieve corresponding vertex.
                faces.emplace_back(
                    dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(tempVertexMap[cubeEBOFaceData[i*3]])),
                    dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(tempVertexMap[cubeEBOFaceData[i*3 + 1]])),
                    dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(tempVertexMap[cubeEBOFaceData[i*3 + 2]])));
            }

            //Generate edges and associate them with correct vertices
            for (int i=0; i<18; i++)//Total number of edges
            {
                edges.emplace_back(
                    dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(tempVertexMap[cubeEBOEdgeData[i*2]])),
                    dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(tempVertexMap[cubeEBOEdgeData[i*2 + 1]])));
            }
        }

        //Populate VBOs
        num::Vec3 pos;
        num::Vec3 normal;
        for (const auto& vertex: vertices) {
            verticesVBOData.push_back((float)vertex.getID());
            pos = vertex.getPos();
            verticesVBOData.push_back(pos.x);
            verticesVBOData.push_back(pos.y);
            verticesVBOData.push_back(pos.z);
        }

        for (auto& face: faces)
        {
            for (int i=0; i<3; i++)
            {
                meshVBOData.push_back((float)face.getID());
                pos = face.getVertices()[i]->getPos();
                meshVBOData.push_back(pos.x);
                meshVBOData.push_back(pos.y);
                meshVBOData.push_back(pos.z);
                normal = face.getNormal();
                meshVBOData.push_back(normal.x);
                meshVBOData.push_back(normal.y);
                meshVBOData.push_back(normal.z);
            }
        }

        for (auto& edge: edges)
        {
            for (int i=0; i<2; i++)
            {
                edgesVBOData.push_back((float)edge.getID());
                pos = edge.getVertices()[i]->getPos();
                edgesVBOData.push_back(pos.x);
                edgesVBOData.push_back(pos.y);
                edgesVBOData.push_back(pos.z);
            }
        }

        //Mesh
        glBindVertexArray(meshVAO);
        glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
        glBufferData(GL_ARRAY_BUFFER, meshVBOData.size()*sizeof(float), meshVBOData.data(), GL_DYNAMIC_DRAW);
        //ID
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(sizeof(float)));
        glEnableVertexAttribArray(1);
        //Normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(4*sizeof(float)));
        glEnableVertexAttribArray(2);

        //Vertices
        glBindVertexArray(verticesVAO);
        glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
        glBufferData(GL_ARRAY_BUFFER, verticesVBOData.size()*sizeof(float), verticesVBOData.data(), GL_DYNAMIC_DRAW);
        //ID
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(sizeof(float)));
        glEnableVertexAttribArray(1);

        //Edges
        glBindVertexArray(edgesVAO);
        glBindBuffer(GL_ARRAY_BUFFER, edgesVBO);
        glBufferData(GL_ARRAY_BUFFER, edgesVBOData.size()*sizeof(float), edgesVBOData.data(), GL_DYNAMIC_DRAW);
        //ID
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void Object3D::renderMesh()
    {
        glBindVertexArray(meshVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3*faces.size());
    }

    void Object3D::renderVertices()
    {
        glBindVertexArray(verticesVAO);
        glDrawArrays(GL_POINTS, 0, vertices.size());
    }

    void Object3D::renderEdges()
    {
        glBindVertexArray(edgesVAO);
        glDrawArrays(GL_LINES, 0, 18*edges.size());
    }

    const float Object3D::cubeVertexPosData[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    };

    //This is not actually sent to an EBO, but used to generate VBO data.
    const int Object3D::cubeEBOEdgeData[] = 
    {
        0,1,1,2,2,3,3,0,        //Bottom edges
        4,5,5,6,6,7,7,4,        //Top edges
        0,4,1,5,2,6,3,7,        //Side edges
        0,5,5,2,3,6,0,7,0,2,4,6,//Diagonal (face) edges
    };

    //This is not actually sent to an EBO, but used to generate VBO data.
    const int Object3D::cubeEBOFaceData[] =
    {
        0, 2, 3,
        0, 1, 2,

        7, 6, 4,
        6, 5, 4,

        0, 4, 5,
        5, 1, 0,

        0, 7, 4,
        0, 3, 7,

        3, 2, 6,
        6, 7, 3,

        5, 6, 2,
        2, 1, 5,
    };
}