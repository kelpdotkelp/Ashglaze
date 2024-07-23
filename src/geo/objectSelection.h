#ifndef OBJECT_SELECTION_H
#define OBJECT_SELECTION_H

namespace geo
{
    struct ObjectSelection
    {
        public:
            int object = -1;
            int vertex = -1;
            int face = -1;
            int edge = -1;

            void reset()
            {
                object = -1;
                vertex = -1;
                face = -1;
                edge = -1;
            }
    };
}

#endif