#include "modelObject.h"

namespace geo
{
    std::unordered_map<unsigned int, ModelObject*> ModelObject::masterObjectMap;
    std::deque<unsigned int> ModelObject::reuseIDDeque;
    unsigned int ModelObject::nextAvailableID = 0;

    ModelObject::ModelObject()
    {
        
    }

    ModelObject::ModelObject(ModelObject* childPtr)
    {
        if (!ModelObject::reuseIDDeque.empty())
        {
            ID = ModelObject::reuseIDDeque.front();
            ModelObject::reuseIDDeque.pop_front();
        }
        else
        {
            ID = nextAvailableID;
            nextAvailableID++;
        }
        
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
        if(masterObjectMap.count(ID) != 0 && masterObjectMap.find(ID) != masterObjectMap.end())
            return masterObjectMap.at(ID);
        else
            return nullptr;
    }

    void ModelObject::masterObjectMapDelete(unsigned int ID)
    {
        ModelObject::masterObjectMap.erase(ID);
        ModelObject::reuseIDDeque.push_back(ID);
    }
}