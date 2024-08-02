#include "vertex.h"

namespace geo
{
    Vertex::Vertex()
    {
        
    }

    Vertex::Vertex(float x, float y, float z) : ModelObject(this)
    {
        //Base class initializes ID
        position = num::Vec3(x, y, z);
    }

    std::string Vertex::toString()
    {
        std::string outStr = "<ID=";
        if (getID() < 10)
            outStr += "00";
        else if (getID() < 100)
            outStr += "0";

        return outStr + std::to_string(getID()) + " Vertex" + ">";
    }

    num::Vec3 Vertex::getPos() const {return position;}
    unsigned int Vertex::getVBOIndex() {return VBOIndex;}

    void Vertex::setVBOIndex(unsigned int VBOIndex) {this->VBOIndex = VBOIndex;}
}