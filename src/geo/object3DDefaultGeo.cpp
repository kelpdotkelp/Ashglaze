#include "object3DDefaultGeo.h"

#include <unordered_map>
#include <iterator>

#include "vertex.h"
#include "edge.h"
#include "face.h"
#include "object3D.h"
#include "num/num.h"

namespace geo
{
    void DefaultGeoGenerator::generateCube(Object3D* object)
    {
        DefaultGeoGenerator::createObject3DDataStructure(object,
            cubeVertices, 8, cubeVerticesIndex, 36);
    }

    void DefaultGeoGenerator::generateOctahedron(Object3D* object)
    {
        DefaultGeoGenerator::createObject3DDataStructure(object,
            octahedronVertices, 6, octahedronVerticesIndex, 24);
    }

    void DefaultGeoGenerator::generateSphere(Object3D* object, int recursionDepth)
    {
        /*  Generates a sphere by starting with a octahedron and recursively subdividing
            each face into Sierpinski triangles. The final step is to project each new vertex
            onto a sphere of radius 1. This algorithm is not actually recursive however, 
            as it would have made building the graph data structure of vertices, edges, and faces difficult.

            The following process repeats for the top and bottom half of the octahedron,
            each time starting at either the top or bottom vertex.

            1. Generate all the vertices, and create a list of all vertices that belong to each layer of the
                of the newly divided octahedron.

            2. Iterate through each layer of vertices to fill in the edges and faces.

            3. Project all the vertices onto a sphere.    
        */
        //Number of triangle layers on each octahedron face
        int layerCount = std::pow(2, recursionDepth);
        //Length of the triangle edges that are on the edge of the original octahedron
        float triangleSideLength = (1.0f/layerCount)*(num::Vec3(-1.0f, 0.0f, -1.0f) - num::Vec3(0.0f, 1.0f, 0.0f)).mag();

        //This vector is used to progess down the edge of the octahedron to get to the next layer of vertices.
        num::Vec3 sideInc = (num::Vec3(-1.0f, 0.0f, -1.0f) - num::Vec3(0.0f, 1.0f, 0.0f));
        sideInc = triangleSideLength*(1.0f/sideInc.mag())*sideInc;
        
        //List that stores pointers to all the vertices in each layer.
        std::vector<std::vector<Vertex*>> vertexLayerLists;

        //Create the vertices for the top half of the octahedron
        sphereGenerateVertices(object, &vertexLayerLists, num::Vec3(0.0f, 1.0f, 0.0f),
            sideInc, layerCount);

        //Use those vertices to generate the edges and faces
        sphereMeshHalf(object, &vertexLayerLists, layerCount);

        //Keep only the middle vertex layer of the octahedron, as we dont want to generate these vertices twice
        std::vector<Vertex*> midLayer = vertexLayerLists.back();
        vertexLayerLists.clear();

        //sideInc now needs to go in the direction of the bottom vertex to one of the middle corners
        sideInc = (num::Vec3(-1.0f, 0.0f, -1.0f) - num::Vec3(0.0f, -1.0f, 0.0f));
        sideInc = triangleSideLength*(1.0f/sideInc.mag())*sideInc;

        //Create the vertices for the bottom half of the octahedron
        //The parameter of 1 prevents the middle vertex layer from being generated again
        sphereGenerateVertices(object, &vertexLayerLists, num::Vec3(0.0f, -1.0f, 0.0f),
            sideInc, layerCount, 1);    

        //Add the middle layer back on as it is required to generate the bottom mesh
        vertexLayerLists.insert(vertexLayerLists.end(), midLayer);  

        //Generate the bottom half of the octahedron
        sphereMeshHalf(object, &vertexLayerLists, layerCount, true);

        //Normalize all vertices
        for (auto& vertex: object->vertices)
        {
            vertex.position = (1.0f/vertex.position.mag())*vertex.position;
        }
    }

    void DefaultGeoGenerator::sphereGenerateVertices(Object3D* object,
                std::vector<std::vector<Vertex*>>* vertexLayerLists, num::Vec3 startingVertex,
                num::Vec3 sideInc, int layerCount, int offset)
    {
        //Length of the triangle edges that are parallel to one of the coordinate axis
        static float triangleLayerDirLength = 
            (1.0f/layerCount)*(num::Vec3(-1.0f, 0.0f, -1.0f) - num::Vec3(1.0f, 0.0f, -1.0f)).mag();

        //Each layer of vertices lie on a square, this vectors are iterated over
        //to reach the next vertex around corners.
        static std::vector<num::Vec3> layerDir =
        {
            triangleLayerDirLength*num::Vec3(1.0f, 0.0f, 0.0f),
            triangleLayerDirLength*num::Vec3(0.0f, 0.0f, 1.0f),
            triangleLayerDirLength*num::Vec3(-1.0f, 0.0f, 0.0f),
            triangleLayerDirLength*num::Vec3(0.0f, 0.0f, -1.0f)
        };

        num::Vec3 curBaseVertex = startingVertex;

        object->vertices.emplace_back(startingVertex.x, startingVertex.y, startingVertex.z);
        vertexLayerLists->insert(vertexLayerLists->end(), {&(object->vertices.back())}); 

        //Generate all vertices.
        for (int vLayer=1; vLayer<layerCount+1 - offset; vLayer++)//Vertex layers
        {
            //Progress down octahedron
            curBaseVertex = curBaseVertex + sideInc;
            num::Vec3 vertexToInsert = curBaseVertex;

            int layerDirIndex = 0;//Keeps track of which direction is being traversed along the vertex layer.

            for (int j=0; j<vLayer*4; j++)
            {
                std::vector<Vertex*>& curList = vertexLayerLists->back();
                object->vertices.emplace_back(vertexToInsert.x, vertexToInsert.y, vertexToInsert.z);

                if (j==0)//Insert new list and new element
                    vertexLayerLists->insert(vertexLayerLists->end(), {&(object->vertices.back())});
                else//Insert just the new vertex
                    curList.insert(curList.end(), &(object->vertices.back()));

                //Determine the vertex to insert on next iteration
                if (j % (vLayer) == 0 && j != 0)
                    layerDirIndex++;
                vertexToInsert = vertexToInsert + layerDir[layerDirIndex];
            }
        }
    }

    void DefaultGeoGenerator::sphereMeshHalf(Object3D* object,
        std::vector<std::vector<Vertex*>>* vertexLayerLists, int layerCount, bool flipNormals)
    {
        for (int i=0; i<layerCount; i++)//Repeat for each layer
        {
            std::vector<Vertex*>& curList = (*vertexLayerLists)[i];
            std::vector<Vertex*>& nextList = (*vertexLayerLists)[i+1];
            
            int triangleCount = 2*i + 1; //2*layerNumber + 1

            for (int k=0; k<4; k++)//Repeat for each face of the octahedron
            {
                //The offsets are used to repeat the same procedure on each octahedron face.
                int curOffset = k*i;//How much to offset the current list, i.e. the start of the face.
                int nextOffset = (i+1)*k;//How much to offset the next list.

                //Fill in one side of the octahedron
                for (int j=0; j<i+1; j++)
                {
                    //Add downwards face
                    Vertex* v1 = curList[(j + curOffset) % curList.size()];;
                    Vertex* v2 = nextList[(j + nextOffset) % nextList.size()];
                    Vertex* v3 = nextList[(j+1 + nextOffset) % nextList.size()];
                    if (!flipNormals)
                        createFace(object, v2, v1, v3);
                    else
                        createFace(object, v1, v2, v3);

                    //Add face to the right of vertex
                    if (j != i)
                    {
                        v1 = curList[(j + curOffset) % curList.size()];
                        v2 = curList[(j+1 + curOffset) % curList.size()];
                        v3 = nextList[(j+1 + nextOffset) % nextList.size()];
                        if (!flipNormals)
                            createFace(object, v1, v2, v3);
                        else
                            createFace(object, v2, v1, v3);
                    }
                }
            }
        }
    }

    void DefaultGeoGenerator::createFace(Object3D* object, Vertex* v1, Vertex* v2, Vertex* v3)
    {
        Edge* e1 = object->edgeExists(v1, v2);
        if (e1 == nullptr)
            e1 = object->addEdgeObject(v1, v2);
        Edge* e2 = object->edgeExists(v2, v3);
        if (e2 == nullptr)
            e2 = object->addEdgeObject(v2, v3);
        Edge* e3 = object->edgeExists(v3, v1);
        if (e3 == nullptr)
            e3 = object->addEdgeObject(v3, v1);

        object->faces.emplace_back(v1, v2, v3, e1, e2, e3);
    }

    void DefaultGeoGenerator::createObject3DDataStructure(Object3D* object,
            float* vertexArray, unsigned int vertexArrayLength,
            int* indexArray, unsigned int indexArrayLength)
    {
        //This is a mapping so the EBO array maps to created vertex IDs
        //It is only used to associate the new vertices with faces.
        std::unordered_map<int, int> tempVertexMap;

        //Generate vertices
        for (int i=0; i<vertexArrayLength; i++)
        {
            //Insert a new vertex into the list
            object->vertices.emplace_back(
                vertexArray[i*3],
                vertexArray[i*3 + 1],
                vertexArray[i*3 + 2]);
            tempVertexMap.insert({i, object->vertices.back().getID()});
        }

        //Generate faces and associate them with correct vertices and edges
        for (int i=0; i<indexArrayLength/3; i++)//6 faces, 2 triangles per face
        {
            //Retrieve EBO index, map that index to masterObjectID, retrieve corresponding vertex.
            Vertex* v1 = dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(
                tempVertexMap[indexArray[i*3]]));
            Vertex* v2 = dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(
                tempVertexMap[indexArray[i*3 + 1]]));
            Vertex* v3 = dynamic_cast<Vertex*>(ModelObject::masterObjectMapGet(
                tempVertexMap[indexArray[i*3 + 2]]));
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
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
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

    float DefaultGeoGenerator::octahedronVertices[] =
    {
        -1.0f,  0.0f, -1.0f,
         1.0f,  0.0f, -1.0f,
         1.0f,  0.0f,  1.0f,
        -1.0f,  0.0f,  1.0f,
         0.0f,  1.0f,  0.0f,
         0.0f, -1.0f,  0.0f
    };

    int DefaultGeoGenerator::octahedronVerticesIndex[] =
    {
        1, 0, 4,
        2, 1, 4,
        4, 3, 2,
        4, 0, 3,
        5, 0, 1,
        1, 2, 5,
        2, 3, 5,
        3, 0, 5
    };
}