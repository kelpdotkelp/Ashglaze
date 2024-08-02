#include "object3D.h"

namespace geo
{
    Object3D::Object3D()
    {

    }

    Object3D::Object3D(BasePrimitives type): ModelObject(this)
    {
        verticesGL = GLDataVertex(4);
        verticesGL.generateGLObjects();
        edgesGL = GLDataEdge(4);
        edgesGL.generateGLObjects();
        facesGL = GLDataFace(7);
        facesGL.generateGLObjects();

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

        verticesGL.generateVBOData(&vertices);
        edgesGL.generateVBOData(&edges);
        facesGL.generateVBOData(&faces);

        verticesGL.configureGLVertexAttribs();
        edgesGL.configureGLVertexAttribs();
        facesGL.configureGLVertexAttribs();

        verticesGL.sendDataToGPU();
        edgesGL.sendDataToGPU();
        facesGL.sendDataToGPU();
    }

    void Object3D::renderMesh() {facesGL.render();}
    void Object3D::renderVertices() {verticesGL.render();}
    void Object3D::renderEdges() {edgesGL.render();}

    void Object3D::translateVertex(Vertex* vertex, num::Vec3 transAmount)
    {
        //Vertex translation
        vertex->position = vertex->position + transAmount;
        //Update all VBOs with new vertex position
        //vertex->VBOIndex is the index that vertex starts in the VBO array
        //***IMPORTANT*** offset VBOIndex by 1 because first value is ID! 
        verticesGL.setPos(vertex->VBOIndex, vertex->position);

        //Update edgesVBOData
        for(auto edgeBelongingTo: vertex->edges)
        {
            // = 1*edgesVBOStride (offset) if second edge vertex must update
            if (edgeBelongingTo->vertex0() == vertex)
                edgesGL.setPos(edgeBelongingTo->VBOIndex, vertex->position, 0);
            else if (edgeBelongingTo->vertex1() == vertex)
                edgesGL.setPos(edgeBelongingTo->VBOIndex, vertex->position, 1);
        }

        //Update facesVBOData
        for (auto faceBelongingTo: vertex->faces)
        {
            unsigned int whichVertex = 0;// = (1 or 2)*meshVBOStride
            for (int i=0; i<3; i++)
            {
                if (faceBelongingTo->getWindingOrder()[i] == vertex)
                {
                    facesGL.setPos(faceBelongingTo->VBOIndex, vertex->position, i);
                    break;
                }
            }

            //Update surface normals
            num::Vec3 normal = faceBelongingTo->getNormal();
            facesGL.setNormal(faceBelongingTo->VBOIndex, faceBelongingTo->getNormal());
        }

        verticesGL.sendDataToGPU();
        edgesGL.sendDataToGPU();
        facesGL.sendDataToGPU();
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

            //Get original faces
            auto faceIter = selectedEdge->faces.begin();
            Face* oldFace0 = *faceIter;
            faceIter++;
            Face* oldFace1 = *faceIter;

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
            }

            deleteFaceObject(oldFace0);
            deleteFaceObject(oldFace1);
            deleteEdgeObject(selectedEdge);

            verticesGL.generateVBOData(&vertices);
            edgesGL.generateVBOData(&edges);
            facesGL.generateVBOData(&faces);
            verticesGL.sendDataToGPU();
            edgesGL.sendDataToGPU();
            facesGL.sendDataToGPU();
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