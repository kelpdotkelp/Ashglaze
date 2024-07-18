#include "modelObject.h"
#include <iostream>
namespace geo
{
    std::unordered_map<unsigned int, ModelObject*> ModelObject::masterObjectMap;
    unsigned int ModelObject::nextAvailableID = 0;

    ModelObject::ModelObject()
    {
        
    }

    ModelObject::ModelObject(ModelObject* childPtr)
    {
        ID = nextAvailableID;
        nextAvailableID++;
        
        masterObjectMap.insert({ID, childPtr});
    }

    unsigned int ModelObject::getID() const {return ID;}

    std::string ModelObject::masterObjectMapToString()
    {
        std::string out = "";
        for (int i = 0; i < masterObjectMap.size(); i++)
        {
            out += masterObjectMap.at(i)->toString();
            if (i != masterObjectMap.size()-1)
                out += "\n";
        }

        return out;
    }

    ModelObject* ModelObject::masterObjectMapGet(unsigned int ID)
    {
        if(masterObjectMap.count(ID) != 0)
            return masterObjectMap.at(ID);
        else
            return nullptr;
    }
}