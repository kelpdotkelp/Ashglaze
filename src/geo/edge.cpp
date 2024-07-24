#include "edge.h"

#include "vertex.h"

namespace geo
{
    Edge::Edge(Vertex* v1, Vertex* v2): ModelObject(this)
    {
        vertices.reserve(2);//Ensures vector doesnt reallocate and invalidate references
        vertices.push_back(v1);
        vertices.push_back(v2);

        v1->edges.emplace_back(this);
        v2->edges.emplace_back(this);
    }

    std::vector<Vertex*> Edge::getVertices() {return vertices;}

    std::string Edge::toString()
    {
        std::string IDString = "";
        for (int i=0; i<2; i++) {IDString += std::to_string(vertices[i]->getID());}

        std::string outStr = "<ID=";
        if (getID() < 10)
            outStr += "00";
        else if (getID() < 100)
            outStr += "0";
        return outStr + std::to_string(getID()) + " Edge" + " (vertex IDs=" + IDString + ")>";
    }
}