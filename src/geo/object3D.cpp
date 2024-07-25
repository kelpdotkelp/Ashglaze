#include "object3D.h"

namespace geo
{
    Object3D::Object3D()
    {

    }

    Object3D::Object3D(BasePrimitives type): ModelObject(this)
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

            //Associate faces with their edges
            for (auto& edge: edges)
            {
                std::vector<Vertex*> edgeV = edge.getVertices();

                for (auto& face: faces)
                {
                    std::vector<Vertex*> faceV = face.getVertices();
                    if((faceV[0] == edgeV[0] && faceV[1] == edgeV[1])
                        || (faceV[1] == edgeV[0] && faceV[2] == edgeV[1])
                        || (faceV[2] == edgeV[0] && faceV[0] == edgeV[1])
                        || (faceV[0] == edgeV[1] && faceV[1] == edgeV[0])
                        || (faceV[1] == edgeV[1] && faceV[2] == edgeV[0])
                        || (faceV[2] == edgeV[1] && faceV[0] == edgeV[0])
                    )
                    {
                        face.addEdge(&edge);
                    }
                }
            }
        }

        //Populate VBOs
        num::Vec3 pos;
        num::Vec3 normal;
        unsigned int VBOIndex = 0;
        for (auto& vertex: vertices) {
            verticesVBOData.push_back((float)vertex.getID());
            pos = vertex.getPos();
            verticesVBOData.push_back(pos.x);
            verticesVBOData.push_back(pos.y);
            verticesVBOData.push_back(pos.z);

            vertex.VBOIndex = verticesVBOStride*VBOIndex;
            VBOIndex++;
        }

        VBOIndex = 0;
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

            face.VBOIndex = 3*meshVBOStride*VBOIndex;
            VBOIndex++;
        }

        VBOIndex = 0;
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

            edge.VBOIndex = 2*edgesVBOStride*VBOIndex;
            VBOIndex++;
        }

        //Mesh
        glBindVertexArray(meshVAO);
        glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
        glBufferData(GL_ARRAY_BUFFER, meshVBOData.size()*sizeof(float), meshVBOData.data(), GL_DYNAMIC_DRAW);
        //ID
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, meshVBOStride*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, meshVBOStride*sizeof(float), (void*)(sizeof(float)));
        glEnableVertexAttribArray(1);
        //Normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, meshVBOStride*sizeof(float), (void*)(4*sizeof(float)));
        glEnableVertexAttribArray(2);

        //Vertices
        glBindVertexArray(verticesVAO);
        glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
        glBufferData(GL_ARRAY_BUFFER, verticesVBOData.size()*sizeof(float), verticesVBOData.data(), GL_DYNAMIC_DRAW);
        //ID
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, verticesVBOStride*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, verticesVBOStride*sizeof(float), (void*)(sizeof(float)));
        glEnableVertexAttribArray(1);

        //Edges
        glBindVertexArray(edgesVAO);
        glBindBuffer(GL_ARRAY_BUFFER, edgesVBO);
        glBufferData(GL_ARRAY_BUFFER, edgesVBOData.size()*sizeof(float), edgesVBOData.data(), GL_DYNAMIC_DRAW);
        //ID
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, edgesVBOStride*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, edgesVBOStride*sizeof(float), (void*)(sizeof(float)));
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

    void Object3D::translateVertex(unsigned int geoID, num::Vec3 transAmount)
    {
        //Vertex translation
        Vertex* vertex = dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(geoID));
        vertex->position = vertex->position + transAmount;
        //Update all VBOs with new vertex position
        //Update verticesVBOData
        //***IMPORTANT*** offset VBOIndex by 1 because first value is ID! 
        verticesVBOData[vertex->VBOIndex+1] = vertex->position.x;
        verticesVBOData[vertex->VBOIndex+2] = vertex->position.y;
        verticesVBOData[vertex->VBOIndex+3] = vertex->position.z;

        //Update edgesVBOData
        for(auto edgeBelongingTo: vertex->edges)
        {
            unsigned int whichVertex = 0;// = 1*edgesVBOStride (offset) if second edge vertex must update
            if (edgeBelongingTo->vertices[0]->getID() == vertex->getID())
                whichVertex = 0;
            else if (edgeBelongingTo->vertices[1]->getID() == vertex->getID())
                whichVertex = edgesVBOStride;
            //***IMPORTANT*** offset VBOIndex by 1 because first value is ID! 
            edgesVBOData[edgeBelongingTo->VBOIndex+1 + whichVertex] = vertex->position.x;
            edgesVBOData[edgeBelongingTo->VBOIndex+2 + whichVertex] = vertex->position.y;
            edgesVBOData[edgeBelongingTo->VBOIndex+3 + whichVertex] = vertex->position.z;
        }

        //Update facesVBOData
        for (auto faceBelongingTo: vertex->faces)
        {

            unsigned int whichVertex = 0;// = (1 or 2)*meshVBOStride
            for (int i=0; i<3; i++)
            {
                if (faceBelongingTo->vertices[i]->getID() == vertex->getID())
                {
                    whichVertex = i*meshVBOStride;
                    break;
                }
            }
            //***IMPORTANT*** offset VBOIndex by 1 because first value is ID! 
            meshVBOData[faceBelongingTo->VBOIndex+1 + whichVertex] = vertex->position.x;
            meshVBOData[faceBelongingTo->VBOIndex+2 + whichVertex] = vertex->position.y;
            meshVBOData[faceBelongingTo->VBOIndex+3 + whichVertex] = vertex->position.z;

            //Update surface normals
            num::Vec3 normal = faceBelongingTo->getNormal();
            for (int i=0; i<3; i++)
            {
                meshVBOData[faceBelongingTo->VBOIndex+4 + i*meshVBOStride] = normal.x;
                meshVBOData[faceBelongingTo->VBOIndex+5 + i*meshVBOStride] = normal.y;
                meshVBOData[faceBelongingTo->VBOIndex+6 + i*meshVBOStride] = normal.z;
            }
        }

        //Update VBO data on GPU
        glBindVertexArray(verticesVAO);
        glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
        glBufferData(GL_ARRAY_BUFFER, verticesVBOData.size()*sizeof(float), verticesVBOData.data(), GL_DYNAMIC_DRAW);
        glBindVertexArray(edgesVAO);
        glBindBuffer(GL_ARRAY_BUFFER, edgesVBO);
        glBufferData(GL_ARRAY_BUFFER, edgesVBOData.size()*sizeof(float), edgesVBOData.data(), GL_DYNAMIC_DRAW);
        glBindVertexArray(meshVAO);
        glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
        glBufferData(GL_ARRAY_BUFFER, meshVBOData.size()*sizeof(float), meshVBOData.data(), GL_DYNAMIC_DRAW);
    }

    void Object3D::translateGeoFeature(unsigned int geoID, num::Vec3 transAmount)
    {
        if(dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(geoID)) != nullptr)
            translateVertex(geoID, transAmount);
        Edge* e = dynamic_cast<Edge*>(ModelObject::masterObjectMapGet(geoID));
        if(e != nullptr)
        {
            translateVertex(e->vertices[0]->getID(), transAmount);
            translateVertex(e->vertices[1]->getID(), transAmount);
        }
        Face* f = dynamic_cast<Face*>(ModelObject::masterObjectMapGet(geoID));   
        if(f != nullptr)
        {
            translateVertex(f->vertices[0]->getID(), transAmount);
            translateVertex(f->vertices[1]->getID(), transAmount);
            translateVertex(f->vertices[2]->getID(), transAmount);
        }   
    }

    std::string Object3D::toString()
    {
        std::string outStr = "<ID=";
        if (getID() < 10)
            outStr += "00";
        else if (getID() < 100)
            outStr += "0";

        return outStr + std::to_string(getID()) + " Object3D" + ">";
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