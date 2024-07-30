#include "edge.h"

#include "vertex.h"

namespace geo
{
    Edge::Edge(Vertex* v1, Vertex* v2): ModelObject(this)
    {
        vertices.insert(v1);
        vertices.insert(v2);

        v1->edges.insert(this);
        v2->edges.insert(this);
    }

    std::unordered_set<Vertex*> Edge::getVertices() {return vertices;}

    Vertex* Edge::vertex0() {return *vertices.begin();}
    Vertex* Edge::vertex1() 
    {
        auto iter = vertices.begin();
        iter++;
        return *(iter);
    }

    num::Vec3 Edge::getMidpoint()
    {
        num::Vec3 midpoint = num::Vec3(0.0f, 0.0f, 0.0f);
        for (auto vertex = vertices.begin(); vertex != vertices.end(); vertex++)
        {
            midpoint = midpoint + num::Vec3((*vertex)->position.x, (*vertex)->position.y, (*vertex)->position.z);
        }

        return 0.5f*midpoint;
    }

    std::string Edge::toString()
    {
        std::string IDString = "";
        for (auto vertex = vertices.begin(); vertex != vertices.end(); vertex++)
        {
            IDString += std::to_string((*vertex)->getID());
        }

        std::string outStr = "<ID=";
        if (getID() < 10)
            outStr += "00";
        else if (getID() < 100)
            outStr += "0";
        return outStr + std::to_string(getID()) + " Edge" + " (vertex IDs=" + IDString + ")>";
    }
}