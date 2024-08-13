#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>

#include "shader.h"
#include "num/num.h"

class ShaderProgram
{
    private:
        unsigned int ID;

        void createShaderProgram(Shader& vertex, Shader& fragment);
        void createShaderProgram(Shader& vertex, Shader& fragment, Shader& geometry);
        void linkProgram();

    public:
        ShaderProgram() {}
        ShaderProgram(Shader& vertex, Shader& fragment);
        ShaderProgram(std::string vertex, std::string fragment);
        ShaderProgram(std::string vertex, std::string fragment, std::string geometry);

        void use();

        void setBool(const std::string& name, bool value);
        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);
        void setVec3(const std::string& name, float f1, float f2, float f3);
        void setVec3(const std::string& name, num::Vec3 vec);
        void setMat4(const std::string& name, num::Mat4 mat);

        int getID();
};

#endif