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

            //Generate faces and associate them with correct vertices and edges
            for (int i=0; i<12; i++)//6 faces, 2 triangles per face
            {
                //Retrieve EBO index, map that index to masterObjectID, retrieve corresponding vertex.
                Vertex* v1 = dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(tempVertexMap[cubeFaceData[i*3]]));
                Vertex* v2 = dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(tempVertexMap[cubeFaceData[i*3 + 1]]));
                Vertex* v3 = dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(tempVertexMap[cubeFaceData[i*3 + 2]]));
                faces.emplace_back(v1, v2, v3);

                //Construct the needed edges
                std::list< std::pair<Vertex*, Vertex*> > vertexPairs = {{v1, v2}, {v1, v3}, {v2, v3}};
                for (auto item: vertexPairs)
                {
                    Edge* edgeToUse = edgeExists(item.first, item.second);
                    if (edgeToUse == nullptr)
                        edgeToUse = addEdgeObject(item.first, item.second);

                    associateEdgeWithFace(edgeToUse, &faces.back());
                }      
            }
        }

        //Populate VBOs
        generateVerticesVBOData();
        generateEdgesVBOData();
        generateFacesVBOData();

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

    void Object3D::translateVertex(Vertex* vertex, num::Vec3 transAmount)
    {
        //Vertex translation
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
            if (edgeBelongingTo->vertex0() == vertex)
                whichVertex = 0;
            else if (edgeBelongingTo->vertex1() == vertex)
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
                if (faceBelongingTo->getWindingOrder()[i] == vertex)
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

        sendVBOToGPU();
    }

    void Object3D::translateGeoFeature(unsigned int geoID, num::Vec3 transAmount)
    {
        Vertex* v = dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(geoID)); 
        if(v != nullptr)
        {
            translateVertex(v, transAmount);
            return;
        }
        Edge* e = dynamic_cast<Edge*>(ModelObject::masterObjectMapGet(geoID));
        if(e != nullptr)
        {
            translateVertex(e->vertex0(), transAmount);
            translateVertex(e->vertex1(), transAmount);
            return;
        }
        Face* f = dynamic_cast<Face*>(ModelObject::masterObjectMapGet(geoID));   
        if(f != nullptr)
        {
            translateVertex(f->getWindingOrder()[0], transAmount);
            translateVertex(f->getWindingOrder()[1], transAmount);
            translateVertex(f->getWindingOrder()[2], transAmount);
            return;
        }   
    }

    void Object3D::insertVertex(unsigned int geoID)
    {
        Edge* selectedEdge = dynamic_cast<Edge*>(ModelObject::masterObjectMapGet(geoID));
        if (selectedEdge != nullptr)//Attempting to insert a vertex along an edge
        {
            //Split original edge into 2 new edges and 1 new vertex
            Vertex* newVertex = addVertexObject(selectedEdge->getMidpoint());
            Edge* newEdgeBreak0 = addEdgeObject(newVertex, selectedEdge->vertex0());
            Edge* newEdgeBreak1 = addEdgeObject(newVertex, selectedEdge->vertex1());
            //The faces for newVertex, newEdgeBreak0, newEdgeBreak1 have not been set.

            for (auto originalFace: selectedEdge->faces)//An edge only ever has 2 faces
            {
                //Vertex of face not contained in the selected edge
                Vertex* thirdVertex = originalFace->getThirdVertex(selectedEdge->vertex0(), selectedEdge->vertex1());
                Edge* edgeThatSplitsFace = addEdgeObject(newVertex, thirdVertex);

                //This constructor associates edges with their faces
                //Winding order is the same as original face but the new vertex replaces the vertex that is not
                //used by the new split face.
                for (auto selectedEdgeVertex: selectedEdge->vertices)//For the two new faces that are created
                {
                    //Get index of original face vertex that is NOT thirdVertex or vertex0
                    int unusedVertexIndex = 0;
                    for (auto v: originalFace->windingOrder)
                    {
                        if (v != thirdVertex && v != selectedEdgeVertex)
                            break;
                        unusedVertexIndex++;
                    }
                    std::vector<Vertex*> newWinding = originalFace->windingOrder;
                    newWinding[unusedVertexIndex] = newVertex;

                    Face* newFace0 = addFaceObject(newWinding[0], newWinding[1], newWinding[2], 
                        edgeExists(newVertex, selectedEdgeVertex), edgeThatSplitsFace, edgeExists(thirdVertex, selectedEdgeVertex));
                }

                deleteFaceObject(originalFace);
            }

            deleteEdgeObject(selectedEdge);

            generateVerticesVBOData();
            generateEdgesVBOData();
            generateFacesVBOData();
            sendVBOToGPU();
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

    void Object3D::generateVerticesVBOData()
    {
        verticesVBOData.clear();//Remove list contents

        unsigned int VBOIndex = 0;
        num::Vec3 pos;
        for (auto& vertex: vertices)
        {
            verticesVBOData.push_back((float)vertex.getID());
            pos = vertex.getPos();
            verticesVBOData.push_back(pos.x);
            verticesVBOData.push_back(pos.y);
            verticesVBOData.push_back(pos.z);

            vertex.VBOIndex = verticesVBOStride*VBOIndex;
            VBOIndex++;
        }
    }

    void Object3D::generateEdgesVBOData()
    {
        edgesVBOData.clear();//Remove list contents

        unsigned int VBOIndex = 0;
        num::Vec3 pos;
        for (auto& edge: edges)
        {
            edgesVBOData.push_back((float)edge.getID());
            pos = edge.vertex0()->getPos();
            edgesVBOData.push_back(pos.x);
            edgesVBOData.push_back(pos.y);
            edgesVBOData.push_back(pos.z);

            edgesVBOData.push_back((float)edge.getID());
            pos = edge.vertex1()->getPos();
            edgesVBOData.push_back(pos.x);
            edgesVBOData.push_back(pos.y);
            edgesVBOData.push_back(pos.z);

            edge.VBOIndex = 2*edgesVBOStride*VBOIndex;
            VBOIndex++;
        }

    }

    void Object3D::generateFacesVBOData()
    {
        meshVBOData.clear();//Remove list contents

        unsigned int VBOIndex = 0;
        num::Vec3 pos;
        num::Vec3 normal;
        for (auto& face: faces)
        {
            for (int i=0; i<3; i++)
            {
                meshVBOData.push_back((float)face.getID());
                pos = face.getWindingOrder()[i]->getPos();
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
    }

    void Object3D::sendVBOToGPU()
    {
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

    Vertex* Object3D::addVertexObject(num::Vec3 position)
    {
        vertices.emplace_back(position.x, position.y, position.z);
        return &vertices.back();
    }

    Edge* Object3D::addEdgeObject(Vertex* v1, Vertex* v2)
    {
        edges.emplace_back(v1, v2);
        return &edges.back();
    }

    Face* Object3D::addFaceObject(Vertex* v1, Vertex* v2, Vertex* v3,
        Edge* e1, Edge* e2, Edge* e3)
    {
        if (e1 == nullptr && e2 == nullptr && e3 == nullptr)
            faces.emplace_back(v1, v2, v3);
        else
            faces.emplace_back(v1, v2, v3, e1, e2, e3);
        return &faces.back();
    }

    void Object3D::deleteEdgeObject(Edge* edge)
    {
        ModelObject::masterObjectMapDelete(edge->getID());//Remove the edge from masterObjectMap
        for (auto iter = edges.begin(); iter != edges.end();)//Remove the edge from Object3D edges list
        {
            if (iter->getID() == edge->getID())
            {
                iter = edges.erase(iter);
                break;
            }
            else
                iter++;
        }
    }

    void Object3D::deleteFaceObject(Face* face)
    {
        ModelObject::masterObjectMapDelete(face->getID());//Remove the edge from masterObjectMap
        for (auto iter = faces.begin(); iter != faces.end();)//Remove the edge from Object3D edges list
        {
            if (iter->getID() == face->getID())
            {
                iter = faces.erase(iter);
                break;
            }
            else
                iter++;
        }
    }

    Edge* Object3D::edgeExists(Vertex* v1, Vertex* v2)
    {
        for (auto edge: v1->edges)
        {
            if(edge->vertex0() == v2 || edge->vertex1() == v2)
                return edge;
        }
        return nullptr;
    }

    void Object3D::associateEdgeWithFace(Edge* edge, Face* face)
    {
        edge->faces.insert(face);
        face->edges.insert(edge);
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

    //Remember that index order specifies the winding.
    const int Object3D::cubeFaceData[] =
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