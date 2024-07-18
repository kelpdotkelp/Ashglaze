#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "num/num.h"
#include "shader.h"
#include "shaderProgram.h"
#include "window.h"
#include "framebufferGeoSelect.h"
#include "geo/modelObject.h"
#include "geo/vertex.h"
#include "geo/face.h"
#include "geo/primitive3D.h"
#include "input/inputManager.h"
#include "input/key.h"

/*      TODO
        use a queue system for nextAvailableID in modelObject.
        use a compute shader to calculate surface normals?
        make directional light a class that configures appropriate shaders.

        unbind left mouse click in fly mode, add erasure functions to inputManager.
        fix flying so camera doesnt jump when switching modes.

        VBO -> vertices pos, ID, (no EBO)
        VBO -> edges    pos, ID, (EBO?)
        VBO -> faces    pos, ID, normals (no EBO)
*/

int initWindowWidth = 800;
int initWindowHeight = 600;

unsigned int selectedObjectID;
bool isFly = false;

void mainRender(bool renderIDMode);
void onObjectSelect();
void onWindowResize(int width, int height);
void onFlyModeSwitch();

ShaderProgram spMesh;
ShaderProgram spVertex;
ShaderProgram spWireframe;
Window* window = nullptr;
input::InputManager inputManager;
FramebufferGeoSelect framebufferGeoSelect;

Camera camera(num::Vec2(initWindowWidth/2.0f, initWindowHeight/2.0f));

geo::Primitive3D cube;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = new Window("Clay", initWindowWidth, initWindowHeight);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    //Default to fly mode
    onFlyModeSwitch();

    framebufferGeoSelect = FramebufferGeoSelect(initWindowWidth, initWindowHeight);
    framebufferGeoSelect.setupDebugDrawing("shaders/fbGeoSelectVert.glsl", "shaders/fbGeoSelectFrag.glsl");

    inputManager.setGLFWWindow(window->getGLFWWindow());
    inputManager.setRenderFunc(std::bind(&mainRender, false));
    inputManager.setWindowResizeFunc(onWindowResize);
    inputManager.registerMouseLeftPress(onObjectSelect);
    inputManager.registerKey(std::bind(&Camera::moveForward, &camera), GLFW_KEY_W, input::InputType::POLLED, input::TriggerType::CONTINUOUS);
    inputManager.registerKey(std::bind(&Camera::moveLeft, &camera), GLFW_KEY_A, input::InputType::POLLED, input::TriggerType::CONTINUOUS);
    inputManager.registerKey(std::bind(&Camera::moveBackward, &camera), GLFW_KEY_S, input::InputType::POLLED, input::TriggerType::CONTINUOUS);
    inputManager.registerKey(std::bind(&Camera::moveRight, &camera), GLFW_KEY_D, input::InputType::POLLED, input::TriggerType::CONTINUOUS);
    inputManager.registerKey(std::bind(&Camera::moveDown, &camera), GLFW_KEY_LEFT_SHIFT, input::InputType::POLLED, input::TriggerType::CONTINUOUS);
    inputManager.registerKey(std::bind(&Camera::moveUp, &camera), GLFW_KEY_SPACE, input::InputType::POLLED, input::TriggerType::CONTINUOUS);
    inputManager.registerKey(std::bind(&Window::closeWindow, window), GLFW_KEY_Q, input::InputType::EVENT, input::TriggerType::SINGLE_PRESS);
    inputManager.registerKey(onFlyModeSwitch, GLFW_KEY_F, input::InputType::EVENT, input::TriggerType::SINGLE_PRESS);

    spMesh = ShaderProgram("shaders/meshVert.glsl", "shaders/meshFrag.glsl");
    spVertex = ShaderProgram("shaders/vertexVert.glsl", "shaders/vertexFrag.glsl");
    spWireframe = ShaderProgram("shaders/wireframeVert.glsl", "shaders/wireframeFrag.glsl");

    spMesh.use();
    spMesh.setVec3("meshColor", 0.5, 0.5, 0.5);
    //Configure directional lighting
    spMesh.setVec3("directionalLight.direction", 0.4, -0.6, -0.4);
    spMesh.setVec3("directionalLight.ambient",   0.8,  0.8, 0.8);
    spMesh.setVec3("directionalLight.diffuse",   1.0,  1.0, 1.0);

    cube = geo::Primitive3D(geo::BasePrimitives::CUBE);

    camera.setPos(0.0, 0.0, 0.0);

    while(!window->shouldClose())
    {
        if (isFly)
        {
            camera.setSpeed(3*window->getDeltaTime());
            camera.updateDirection(num::Vec2(inputManager.getMousePosX(), inputManager.getMousePosY()));
        }

        mainRender(false);

        window->swapBuffers();
        window->updateDeltaTime();
        inputManager.pollInputs();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void mainRender(bool renderIDMode)
{
    static float angle = 0.0f;
    //angle += 25.0*window->getDeltaTime();
    if (angle > 360)
        angle = 0.0f;

    if(!renderIDMode)
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    else
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    num::Mat4 projection = num::perspective(window->getAspectRatio(), 70, 0.1f, 100.0f);
    num::Mat4 view = camera.getViewMatrix();

    num::Mat4 model = num::Mat4();
    //model = num::rotateX(model, angle);
    model = num::rotateY(model, -angle);
    model = num::translate(model, 0.0f, 0.0f, 0.0f);

    //Mesh
    spMesh.use();
    spMesh.setMat4("model", model);
    spMesh.setMat4("view", view);
    spMesh.setMat4("projection", projection);
    cube.renderMesh();

    spWireframe.use();
    spWireframe.setMat4("model", model);
    spWireframe.setMat4("view", view);
    spWireframe.setMat4("projection", projection);
    glLineWidth(2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    cube.renderMesh();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Render selected vertices
    spVertex.use();
    spVertex.setBool("renderIDMode", renderIDMode);
    spVertex.setVec3("color", 0.0f, 0.0f, 1.0f);

    spVertex.setMat4("model", model);
    spVertex.setMat4("view", view);
    spVertex.setMat4("projection", projection);
    cube.renderVertices();
}

void onObjectSelect()
{
    //Render the geometry selection framebuffer
    framebufferGeoSelect.bind();
    glDisable(GL_DITHER);
    mainRender(true);
    glEnable(GL_DITHER);

    float pixels[3];
    num::Vec2 mousePos = num::Vec2(inputManager.getMousePosX(), inputManager.getMousePosY());
    glFinish();
    glReadPixels(mousePos.x, window->getHeight() - mousePos.y, 1, 1, GL_RGB, GL_FLOAT, &pixels);
    selectedObjectID = pixels[0]*256 + pixels[1]*256*256 + pixels[2] * 256*256*256;

    if (geo::ModelObject::masterObjectMapGet(selectedObjectID) != nullptr)
        std::cout << geo::ModelObject::masterObjectMapGet(selectedObjectID)->toString() << std::endl;

    framebufferGeoSelect.unbind();
}

void onWindowResize(int width, int height)
{
    window->setHeight(height);
    window->setWidth(width);
    framebufferGeoSelect.updateBufferSize(width, height);
}

void onFlyModeSwitch()
{
    static num::Vec2 mousePos;

    isFly = (isFly + 1) % 2;

    if (isFly)
    {
        glfwSetCursorPos(window->getGLFWWindow(), mousePos.x, mousePos.y);
        glfwSetInputMode(window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mousePos = num::Vec2(inputManager.getMousePosX(), inputManager.getMousePosY());
        glfwSetCursorPos(window->getGLFWWindow(), window->getWidth()/2.0, window->getHeight()/2.0);
        inputManager.setMousePosition(mousePos.x, mousePos.y);
    }
}