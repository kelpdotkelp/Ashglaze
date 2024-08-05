#include "object3DDefaultGeo.h"

#include <unordered_map>

#include "vertex.h"
#include "edge.h"
#include "face.h"
#include "object3D.h"

namespace geo
{
    void DefaultGeoGenerator::generateCube(Object3D* object)
    {
        //This is a mapping so the EBO array maps to created vertex IDs
        //It is only used to associate the new vertices with faces.
        std::unordered_map<int, int> tempVertexMap;

        //Generate vertices
        for (int i=0; i<8; i++)
        {
            //Insert a new vertex into the list
            object->vertices.emplace_back(
                DefaultGeoGenerator::cubeVertices[i*3],
                DefaultGeoGenerator::cubeVertices[i*3 + 1],
                DefaultGeoGenerator::cubeVertices[i*3 + 2]);
            tempVertexMap.insert({i, object->vertices.back().getID()});
        }

        //Generate faces and associate them with correct vertices and edges
        for (int i=0; i<12; i++)//6 faces, 2 triangles per face
        {
            //Retrieve EBO index, map that index to masterObjectID, retrieve corresponding vertex.
            Vertex* v1 = dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(
                tempVertexMap[DefaultGeoGenerator::cubeVerticesIndex[i*3]]));
            Vertex* v2 = dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(
                tempVertexMap[DefaultGeoGenerator::cubeVerticesIndex[i*3 + 1]]));
            Vertex* v3 = dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(
                tempVertexMap[DefaultGeoGenerator::cubeVerticesIndex[i*3 + 2]]));
            object->faces.emplace_back(v1, v2, v3);

            //Construct the needed edges
            std::list< std::pair<Vertex*, Vertex*> > vertexPairs = {{v1, v2}, {v1, v3}, {v2, v3}};
            for (auto item: vertexPairs)
            {
                Edge* edgeToUse = object->edgeExists(item.first, item.second);
                if (edgeToUse == nullptr)
                    edgeToUse = object->addEdgeObject(item.first, item.second);

                object->associateEdgeWithFace(edgeToUse, &(object->faces.back()));
            }      
        }
    }

    float DefaultGeoGenerator::cubeVertices[] = {
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
    int DefaultGeoGenerator::cubeVerticesIndex[] =
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