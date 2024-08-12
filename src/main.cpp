#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <typeinfo>

#include "camera.h"
#include "num/num.h"
#include "shader.h"
#include "shaderProgram.h"
#include "window.h"
#include "framebufferGeoSelect.h"
#include "geo/modelObject.h"
#include "geo/vertex.h"
#include "geo/face.h"
#include "geo/objectSelection.h"
#include "geo/object3D.h"
#include "input/inputManager.h"
#include "input/key.h"

/*      TODO
        ->  make directional light a class that configures appropriate shaders.
        ->  unbind left mouse click in fly mode, add erasure functions to inputManager.
        ->  add zoom on scroll wheel.
        ->  forward declare all classes that only use pointers
        ->  Object3D store vertices, edges, faces, as an unordered_set for fast lookup/insertion/deletion

        -> Add specular highlights to directional lighting

        -> Euler characteristic for error checking?

        ->Crashing sometimes when hitting Q to exit window

        Lines need to be rendered as quads (use geometry shader?) since OpenGL doesnt require glLineWidth > 1.
*/

int initWindowWidth = 800;
int initWindowHeight = 600;

bool isFly = false;

void mainRender(bool renderIDMode);
void onObjectSelect();
void onWindowResize(int width, int height);
void onFlyModeSwitch();

void TEST_translateGeo(int direction, std::string axis);
void TEST_insertVertex();

ShaderProgram spMesh;
ShaderProgram spVertex;
ShaderProgram spEdge;
ShaderProgram spWireframe;
Window* window = nullptr;
input::InputManager inputManager;
FramebufferGeoSelect framebufferGeoSelect;

Camera camera(num::Vec2(initWindowWidth/2.0f, initWindowHeight/2.0f));

//Maintains which Object3D and which edge/vertex/face has been selected. 
geo::ObjectSelection objectSelected;
geo::Object3D cube;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = new Window("Stoneware3D", initWindowWidth, initWindowHeight);
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

    //Testing geometry translation, do this from UI at a later point.
    inputManager.registerKey(std::bind(&TEST_translateGeo, 1, std::string("X")), GLFW_KEY_T, input::InputType::POLLED, input::TriggerType::CONTINUOUS);
    inputManager.registerKey(std::bind(&TEST_translateGeo, -1, std::string("X")), GLFW_KEY_G, input::InputType::POLLED, input::TriggerType::CONTINUOUS);
    inputManager.registerKey(std::bind(&TEST_translateGeo, 1, std::string("Y")), GLFW_KEY_Y, input::InputType::POLLED, input::TriggerType::CONTINUOUS);
    inputManager.registerKey(std::bind(&TEST_translateGeo, -1, std::string("Y")), GLFW_KEY_H, input::InputType::POLLED, input::TriggerType::CONTINUOUS);
    inputManager.registerKey(std::bind(&TEST_translateGeo, 1, std::string("Z")), GLFW_KEY_U, input::InputType::POLLED, input::TriggerType::CONTINUOUS);
    inputManager.registerKey(std::bind(&TEST_translateGeo, -1, std::string("Z")), GLFW_KEY_J, input::InputType::POLLED, input::TriggerType::CONTINUOUS);
    inputManager.registerKey(TEST_insertVertex, GLFW_KEY_I, input::InputType::EVENT, input::TriggerType::SINGLE_PRESS);

    spMesh = ShaderProgram("shaders/meshVert.glsl", "shaders/meshFrag.glsl");
    spVertex = ShaderProgram("shaders/vertexVert.glsl", "shaders/vertexFrag.glsl");
    spEdge = ShaderProgram("shaders/edgeVert.glsl", "shaders/edgeFrag.glsl");
    spWireframe = ShaderProgram("shaders/wireframeVert.glsl", "shaders/wireframeFrag.glsl");

    spMesh.use();
    spMesh.setVec3("meshColor", 0.5, 0.5, 0.5);
    spMesh.setVec3("selectedColorChange", 0.2, 0.2, 0.2);
    //Configure directional lighting
    spMesh.setVec3("directionalLight0.direction", 0.4, -0.6, -0.4);
    spMesh.setVec3("directionalLight0.ambient",   0.8,  0.8, 0.8);
    spMesh.setVec3("directionalLight0.diffuse",   1.0,  1.0, 1.0);
    spMesh.setVec3("directionalLight1.direction", -0.4, 0.6, 0.4);
    spMesh.setVec3("directionalLight1.ambient",   0.0,  0.0, 0.0);
    spMesh.setVec3("directionalLight1.diffuse",   0.2,  0.2, 0.2);

    spVertex.use();
    spVertex.setVec3("color", 0.0f, 0.0f, 1.0f);
    spVertex.setVec3("selectedColor", 0.0f, 1.0f, 0.0f);

    spEdge.use();
    spEdge.setVec3("color", 0.0f, 0.0f, 1.0f);
    spEdge.setVec3("selectedColor", 0.0f, 1.0f, 0.0f);

    cube = geo::Object3D(geo::BasePrimitives::SPHERE, 3);

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
    spMesh.setMat4("model", model);
    spMesh.setMat4("view", view);
    spMesh.setMat4("projection", projection);
    if (objectSelected.object == cube.getID())
    {
        spMesh.setInt("selectedID", objectSelected.face);//Highlight selected triangle
        spMesh.setInt("selectedIDObject", -1);
    }
    else//Change rendering so entire mesh maps to Object3D.getID()
    {
        spMesh.setInt("selectedID", -1);
        spMesh.setInt("selectedIDObject", cube.getID());
    }
    cube.renderMesh();

    if (!renderIDMode && (objectSelected.object == cube.getID()))
    {
        spWireframe.use();
        spWireframe.setMat4("model", model);
        spWireframe.setMat4("view", view);
        spWireframe.setMat4("projection", projection);
        glLineWidth(2);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        cube.renderMesh();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (objectSelected.object == cube.getID())
    {
        geo::Face* f = dynamic_cast<geo::Face*>(geo::ModelObject::masterObjectMapGet(objectSelected.face));

        //Render selected Edges
        spEdge.use();
        spEdge.setBool("renderIDMode", renderIDMode);
        spEdge.setInt("selectedID", objectSelected.edge);//Highlight selected vertex
        if (f != nullptr)
        {
            spEdge.setVec3("IDToRender", f->edge0()->getID(), f->edge1()->getID(), f->edge2()->getID());
        }
        else
        {
            spEdge.setVec3("IDToRender", -1, -1, -1);
        }
        spEdge.setMat4("model", model);
        spEdge.setMat4("view", view);
        spEdge.setMat4("projection", projection);
        glLineWidth(12.5);
        cube.renderEdges();

        //Render selected vertices
        spVertex.use();
        spVertex.setBool("renderIDMode", renderIDMode);
        spVertex.setInt("selectedID", objectSelected.vertex);//Highlight selected vertex
        //Render only the vertices associated with the selected face
        if (f != nullptr)
        {
            spVertex.setVec3("IDToRender", f->getWindingOrder()[0]->getID(), 
                f->getWindingOrder()[1]->getID(), f->getWindingOrder()[2]->getID());
        }
        else
            spVertex.setVec3("IDToRender", -1, -1, -1);
        spVertex.setMat4("model", model);
        spVertex.setMat4("view", view);
        spVertex.setMat4("projection", projection);
        cube.renderVertices();
    }
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
    //Converting from float RGB ID back to integer ID:
    //  Multiplication by 256 corresponds to bit shifting left by 8
    //  Multiplication by 255 undoes the mapping for a colour 0-255 to 0.0-1.0.
    unsigned int selectedObjectID = pixels[0]*255 + (pixels[1]*256)*255 + (pixels[2] * 256*256)*255;

    if (geo::ModelObject::masterObjectMapGet(selectedObjectID) == nullptr)
    {
        selectedObjectID = -1;
        objectSelected.reset();//Deselect all objects
    }
    else
    {
        //Determine if a Object3D was selected, or if a feature within Object3D was selected.
        if (objectSelected.object == -1)
            objectSelected.object = selectedObjectID;
        else//Object has already been selected, now choose a face/vertex/edge
        {
            if (dynamic_cast<geo::Vertex*>(geo::ModelObject::masterObjectMapGet(selectedObjectID)) != nullptr)
            {
                objectSelected.vertex = selectedObjectID;
                objectSelected.edge = -1;
            }
            else if (dynamic_cast<geo::Edge*>(geo::ModelObject::masterObjectMapGet(selectedObjectID)) != nullptr)   
            {
                objectSelected.edge = selectedObjectID;
                objectSelected.vertex = -1;
            }
            else if (dynamic_cast<geo::Face*>(geo::ModelObject::masterObjectMapGet(selectedObjectID)) != nullptr)
            {
                objectSelected.face = selectedObjectID;
                objectSelected.vertex = -1;
                objectSelected.edge = -1;
            }
        }
        objectSelected.lastSelected = selectedObjectID;
    }

    if (objectSelected.lastSelected != -1)
        std::cout << geo::ModelObject::masterObjectMapGet(objectSelected.lastSelected)->toString() << std::endl;

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

void TEST_translateGeo(int direction, std::string axis)
{
    static float spd = 1.0f;

    if (objectSelected.lastSelected == -1)
        return;

    if (axis == "X")
        cube.translateGeoFeature(objectSelected.lastSelected, direction*num::Vec3(spd*window->getDeltaTime(), 0, 0));
    if (axis == "Y")
        cube.translateGeoFeature(objectSelected.lastSelected, direction*num::Vec3(0, spd*window->getDeltaTime(), 0));
    if (axis == "Z")
        cube.translateGeoFeature(objectSelected.lastSelected, direction*num::Vec3(0, 0, spd*window->getDeltaTime()));
}

void TEST_insertVertex()
{
    if (objectSelected.lastSelected == -1)
        return;

    cube.insertVertex(objectSelected.lastSelected);
}