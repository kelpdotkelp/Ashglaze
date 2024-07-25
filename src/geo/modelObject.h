#ifndef MODEL_OBJECT_H
#define MODEL_OBJECT_H

#include <deque>
#include <iostream>
#include <string>
#include <unordered_map>

namespace geo
{
    class ModelObject
    {
        private:
            //Maintains a map of all modelObjects
            static std::unordered_map<unsigned int, ModelObject*> masterObjectMap;
            static std::deque<unsigned int> reuseIDDeque;
            static unsigned int nextAvailableID;

            unsigned int ID;

        public:
            static std::string masterObjectMapToString();
            static ModelObject* masterObjectMapGet(unsigned int ID);

            ModelObject();
            ModelObject(ModelObject* childPtr);

            unsigned int getID() const;

            virtual std::string toString() = 0;
        
        protected:
            static void masterObjectMapDelete(unsigned int ID);
    };
}

#endif