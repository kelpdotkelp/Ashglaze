#include "shaderProgram.h"

ShaderProgram::ShaderProgram(Shader& vertex, Shader& fragment)
{
    createShaderProgram(vertex, fragment);
}

ShaderProgram::ShaderProgram(std::string vertex, std::string fragment)
{
    Shader shaderVertex = Shader(vertex, GL_VERTEX_SHADER);
    Shader shaderFragment = Shader(fragment, GL_FRAGMENT_SHADER);
    createShaderProgram(shaderVertex, shaderFragment);
    shaderVertex.deleteShader();
    shaderFragment.deleteShader();
}

void ShaderProgram::createShaderProgram(Shader& vertex, Shader& fragment)
{
    ID = glCreateProgram();
    glAttachShader(ID, vertex.getID());
    glAttachShader(ID, fragment.getID());
    glLinkProgram(ID);

    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM Linking failed.\n" <<
            infoLog << std::endl;
    }
}

void ShaderProgram::use()
{
    glUseProgram(ID);
}

void ShaderProgram::setBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void ShaderProgram::setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void ShaderProgram::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void ShaderProgram::setVec3(const std::string& name, float f1, float f2, float f3)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3);
}
void ShaderProgram::setVec3(const std::string& name, num::Vec3 vec)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
}
void ShaderProgram::setMat4(const std::string& name, num::Mat4 mat)
{
    //GL_TRUE to transpose matrix
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_TRUE, &mat.nums[0][0]);
}

int ShaderProgram::getID() {return ID;}