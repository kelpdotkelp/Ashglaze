#ifndef FRAMEBUFFER_GEO_SELECT_H
#define FRAMEBUFFER_GEO_SELECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

#include "shader.h"
#include "shaderProgram.h"

class FramebufferGeoSelect
{
    private:
        const static float quadData[24];

        unsigned int FBO;
        unsigned int RBO;//For depth and stencil testing
        unsigned int textureAttachment;

        bool isDebugRenderSetup = false;
        unsigned int debugVBO;
        unsigned int debugVAO;
        ShaderProgram shaderProgram;

    public:       
        FramebufferGeoSelect();
        FramebufferGeoSelect(int width, int height);

        void setupDebugDrawing(std::string shaderVertexPath, std::string shaderFragmentPath);

        void updateBufferSize(int width, int height);

        void draw(num::Vec2 scale, num::Vec2 position);

        ShaderProgram* getShaderProgram();

        void bind();
        void unbind();
};

#endif