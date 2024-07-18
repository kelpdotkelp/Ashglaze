#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <glad/glad.h>

class Shader
{
    private:
        unsigned int ID;
    
    public:
        Shader(std::string path, int shaderType);

        void deleteShader();

        int getID();
};

#endif