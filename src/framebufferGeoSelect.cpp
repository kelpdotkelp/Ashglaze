#include "framebufferGeoSelect.h"

const float FramebufferGeoSelect::quadData[] = {
    // positions // texCoords
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f
};

FramebufferGeoSelect::FramebufferGeoSelect()
{
    
}

FramebufferGeoSelect::FramebufferGeoSelect(int width, int height)
{
    glGenFramebuffers(1, &FBO);
    glGenRenderbuffers(1, &RBO);
    bind();

    glGenTextures(1, &textureAttachment);
    updateBufferSize(width, height);
    unbind();
}

void FramebufferGeoSelect::setupDebugDrawing(std::string shaderVertexPath, std::string shaderFragmentPath)
{
    //VBO
    glGenBuffers(1, &debugVBO);
    glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadData), quadData, GL_STATIC_DRAW);

    //VAO
    glGenVertexArrays(1, &debugVAO);
    glBindVertexArray(debugVAO);
    glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    //Shaders
    Shader shaderVertex = Shader(shaderVertexPath, GL_VERTEX_SHADER);
    Shader shaderFragment = Shader(shaderFragmentPath, GL_FRAGMENT_SHADER);
    shaderProgram = ShaderProgram(shaderVertex, shaderFragment);
    shaderVertex.deleteShader();
    shaderFragment.deleteShader();

    isDebugRenderSetup = true;
}

void FramebufferGeoSelect::updateBufferSize(int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, textureAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
        GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        textureAttachment, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, RBO);
}

void FramebufferGeoSelect::draw(num::Vec2 scale, num::Vec2 position)
{
    if(!isDebugRenderSetup)
    {
        std::cout << "FramebufferGeoSelect has not been configured to render!" << std::endl;
        return;
    }

    shaderProgram.use();
    glBindVertexArray(debugVAO);
    glDisable(GL_DEPTH_TEST);//Ensure quad renders in front
    glBindTexture(GL_TEXTURE_2D, textureAttachment);

    num::Mat4 model = num::Mat4();
    model = num::scale(model, scale.x, scale.y, 0.0f);
    model = num::translate(model, position.x, position.y, 0.0f);
    getShaderProgram()->setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

ShaderProgram* FramebufferGeoSelect::getShaderProgram() {return &shaderProgram;}

void FramebufferGeoSelect::bind() {glBindFramebuffer(GL_FRAMEBUFFER, FBO);}
void FramebufferGeoSelect::unbind() {glBindFramebuffer(GL_FRAMEBUFFER, 0);}