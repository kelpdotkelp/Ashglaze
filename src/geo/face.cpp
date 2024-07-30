#include "face.h"

namespace geo
{
    Face::Face(Vertex* v1, Vertex* v2, Vertex* v3): ModelObject(this)
    {
        vertices.insert(v1);
        vertices.insert(v2);
        vertices.insert(v3);

        v1->faces.insert(this);
        v2->faces.insert(this);
        v3->faces.insert(this);

        windingOrder.reserve(3);
        windingOrder.emplace_back(v1);
        windingOrder.emplace_back(v2);
        windingOrder.emplace_back(v3);
    }

    Face::Face(Edge* e1, Edge* e2, Edge* e3): ModelObject(this)
    {
        
    }

    std::vector<Vertex*> Face::getWindingOrder() {return windingOrder;}

    Edge* Face::edge0() {return *(edges.begin());}
    Edge* Face::edge1() 
    {
        auto iter = edges.begin();
        iter++;
        return *(iter);
    }
    Edge* Face::edge2() 
    {
        auto iter = edges.begin();
        iter++;
        iter++;
        return *(iter);
    }

    num::Vec3 Face::getNormal()
    {
        num::Vec3 v1 = windingOrder[1]->getPos() - windingOrder[0]->getPos();
        num::Vec3 v2 = windingOrder[2]->getPos() - windingOrder[0]->getPos();
        num::Vec3 cp = num::crossProduct(v1, v2);
        return cp * (1.0/cp.mag());
    }

    Vertex* Face::getThirdVertex(Vertex* vertex0Ptr, Vertex* vertex1Ptr)
    {
        Vertex* thirdVertex;
        for (auto vertex = vertices.begin(); vertex != vertices.end(); vertex++)
        {
            if ((*vertex) != vertex0Ptr && (*vertex) != vertex1Ptr)
            {
                thirdVertex = (*vertex);
                break;
            }
        }
        return thirdVertex;
    }

    /*void Face::addEdge(Edge* edge)
    {
        edges.emplace_back(edge);
        edge->faces.emplace_back(this);
    }*/

    std::string Face::toString()
    {
        std::string IDString = "";
        for (int i=0; i<3; i++) 
        {
            IDString += std::to_string(windingOrder[i]->getID());
        }

        std::string outStr = "<ID=";
        if (getID() < 10)
            outStr += "00";
        else if (getID() < 100)
            outStr += "0";
        return outStr + std::to_string(getID()) + " Face" + " (vertex IDs=" + IDString + ")>";
    }
}