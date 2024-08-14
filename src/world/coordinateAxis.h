#ifndef COORDINATE_AXIS_H
#define COORDINATE_AXIS_H

#include <vector>

class ShaderProgram;
namespace num {class Mat4;}

namespace world
{
    class CoordinateAxis
    {
        private:
            static std::vector<float> VBOData;
            static std::vector<int>   EBOData;

            unsigned int VBO;
            unsigned int VAO;
            unsigned int EBO;

        public:
            CoordinateAxis();

            void render(ShaderProgram& ShaderProgram, num::Mat4 view, num::Mat4 projection);
            void setup();
    };
}

#endif