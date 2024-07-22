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
        add zoom on scroll wheel

        Edges and wireframe need to be specified in the same way so they match
        Lines need to be rendered as quads (use geometry shader?) since OpenGL doesnt require glLineWidth > 1.
        Render lines only if triangle is selected.

        VBO -> vertices pos, ID, (no EBO)
        VBO -> edges    pos, ID, (EBO?)
        VBO -> faces    pos, ID, normals (no EBO)

        Maybe? Although this might be too much memory
        Vertices should reference which edges/faces it belongs to
        Edges should reference which vertices it has and faces it belongs to
        Faces should reference which edges and vertices it has
*/

int initWindowWidth = 800;
int initWindowHeight = 600;

int selectedObjectID = -1;//-1 maps to no object selected.
bool isFly = false;

void mainRender(bool renderIDMode);
void onObjectSelect();
void onWindowResize(int width, int height);
void onFlyModeSwitch();

ShaderProgram spMesh;
ShaderProgram spVertex;
ShaderProgram spEdge;
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
    spEdge = ShaderProgram("shaders/edgeVert.glsl", "shaders/edgeFrag.glsl");
    spWireframe = ShaderProgram("shaders/wireframeVert.glsl", "shaders/wireframeFrag.glsl");

    spMesh.use();
    spMesh.setVec3("meshColor", 0.5, 0.5, 0.5);
    spMesh.setVec3("selectedColorChange", 0.2, 0.2, 0.2);
    //Configure directional lighting
    spMesh.setVec3("directionalLight.direction", 0.4, -0.6, -0.4);
    spMesh.setVec3("directionalLight.ambient",   0.8,  0.8, 0.8);
    spMesh.setVec3("directionalLight.diffuse",   1.0,  1.0, 1.0);

    spVertex.use();
    spVertex.setVec3("color", 0.0f, 0.0f, 1.0f);
    spVertex.setVec3("selectedColor", 0.0f, 1.0f, 0.0f);

    spEdge.use();
    spEdge.setVec3("color", 0.0f, 0.0f, 1.0f);
    spVertex.setVec3("selectedColor", 0.0f, 1.0f, 0.0f);

    cube = geo::Primitive3D(geo::BasePrimitives::CUBE);

    camera.setPos(0.0, 0.0, 0.0);

    while(!window->shouldClose())
    {
        camera.setSpeed(3*window->getDeltaTime());
        camera.updateDirection(num::Vec2(inputManager.getMousePosX(), inputManager.getMousePosY()));

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
    if(!renderIDMode)
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    else
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    num::Mat4 projection = num::perspective(window->getAspectRatio(), 70, 0.1f, 100.0f);
    num::Mat4 view = camera.getViewMatrix();

    num::Mat4 model = num::Mat4();
    model = num::translate(model, 0.0f, 0.0f, 0.0f);

    //Mesh
    spMesh.use();
    spMesh.setBool("renderIDMode", renderIDMode);
    spMesh.setInt("selectedID", selectedObjectID);//Highlight selected triangle
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
    spVertex.setInt("selectedID", selectedObjectID);//Highlight selected vertex
    spVertex.setMat4("model", model);
    spVertex.setMat4("view", view);
    spVertex.setMat4("projection", projection);
    cube.renderVertices();

    //Render selected Edges
    spEdge.use();
    spEdge.setBool("renderIDMode", renderIDMode);
    spEdge.setInt("selectedID", selectedObjectID);//Highlight selected vertex
    spEdge.setMat4("model", model);
    spEdge.setMat4("view", view);
    spEdge.setMat4("projection", projection);
    glLineWidth(12.5);
    cube.renderEdges();
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

    if (geo::ModelObject::masterObjectMapGet(selectedObjectID) == nullptr)
        selectedObjectID = -1;

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
    //Cursor position changes dependent upon glfwSetInputMode.
    //GLFW_CURSOR_DISABLED allows the cursor to take the full range of a double.
    static num::Vec2 mousePos;//Saves last mouse position before camera was disabled
    isFly = (isFly + 1) % 2;

    if (isFly)
    {
        glfwSetInputMode(window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        camera.disable(false);
        inputManager.setMousePosition(mousePos.x, mousePos.y);//This calls glfwSetCursorPos
    }    
    else
    {
        mousePos = num::Vec2(inputManager.getMousePosX(), inputManager.getMousePosY());
        glfwSetInputMode(window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        camera.disable(true);

        //Place cursor in centre of window
        inputManager.setMousePosition(window->getWidth()/2.0f, window->getHeight()/2.0f);
    }
}