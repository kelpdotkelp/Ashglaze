#include "face.h"

namespace geo
{
    Face::Face(Vertex* v1, Vertex* v2, Vertex* v3): ModelObject(this)
    {
        vertices.reserve(3);//Ensures vector doesnt reallocate and invalidate references
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);

        edges.reserve(3);

        v1->faces.emplace_back(this);
        v2->faces.emplace_back(this);
        v3->faces.emplace_back(this);
    }

    std::vector<Vertex*> Face::getVertices() {return vertices;}
    std::vector<Edge*> Face::getEdges() {return edges;}

    num::Vec3 Face::getNormal()
    {
        num::Vec3 v1 = vertices[1]->getPos() - vertices[0]->getPos();
        num::Vec3 v2 = vertices[2]->getPos() - vertices[0]->getPos();
        num::Vec3 cp = num::crossProduct(v1, v2);
        return cp * (1.0/cp.mag());
    }

    unsigned int Face::getThirdVertex(unsigned int vertex0ID, unsigned int vertex1ID)
    {
        unsigned int thirdVertex;
        for (auto vertex: vertices)
        {
            if (vertex->getID() != vertex0ID && vertex->getID() != vertex1ID)
            {
                thirdVertex = vertex->getID();
                break;
            }
        }
        return thirdVertex;
    }

    void Face::addEdge(Edge* edge)
    {
        edges.emplace_back(edge);
        edge->faces.emplace_back(this);
    }

    std::string Face::toString()
    {
        std::string IDString = "";
        for (int i=0; i<3; i++) {IDString += std::to_string(vertices[i]->getID());}

        std::string outStr = "<ID=";
        if (getID() < 10)
            outStr += "00";
        else if (getID() < 100)
            outStr += "0";
        return outStr + std::to_string(getID()) + " Face" + " (vertex IDs=" + IDString + ")>";
    }
}