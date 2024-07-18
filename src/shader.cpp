#include "shader.h"

Shader::Shader(std::string path, int shaderType)
{
    std::ifstream shaderFile;
    std::string shaderString;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderString = shaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::File not successfully read." << std::endl;
    }

    const char* shaderStringC = shaderString.c_str();
    int success;
    char infoLog[512];
    ID = glCreateShader(shaderType);
    glShaderSource(ID, 1, &shaderStringC, NULL);
    glCompileShader(ID);
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::Compilation failed.\n" <<
            infoLog << std::endl; 
    }
}

void Shader::deleteShader() {glDeleteShader(ID);}

int Shader::getID() {return ID;}