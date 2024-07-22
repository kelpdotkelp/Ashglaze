#include "inputManager.h"

namespace input
{
    InputManager::InputManager()
    {

    }

    void InputManager::registerCallbacks()
    {
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetWindowRefreshCallback(window, windowRefreshCallback);
        glfwSetCursorPosCallback(window, mousePositionCallback);
        glfwSetMouseButtonCallback(window, mousePressedCallback);
        glfwSetKeyCallback(window, keyPressCallback);

        glfwSetWindowUserPointer(window, static_cast<void *>(this));
    }

    void InputManager::pollInputs()
    {
        for (auto& it: keyMap)
        {
            if (it.second.inputType == input::InputType::POLLED)
            {
                //GLFW_PRESS or GLFW_RELEASE
                int keyState = glfwGetKey(window, it.first);

                if (keyState == GLFW_PRESS && it.second.triggerType == input::TriggerType::CONTINUOUS)
                    (it.second.keyBindFunc)();
                else if (it.second.triggerType == input::TriggerType::SINGLE_PRESS)
                {
                    if (keyState == GLFW_PRESS && it.second.isSinglePressReady)
                    {
                        (it.second.keyBindFunc)();
                        it.second.isSinglePressReady = false;
                    }
                    if (keyState == GLFW_RELEASE)
                    {
                        it.second.isSinglePressReady = true;
                    }
                }
            }
        }
    }

    /***********
     * 
     * Getters
     * 
    ************/
    GLFWwindow* InputManager::getGLFWWindow() {return window;}
    std::function<void()> InputManager::getRenderFunc() {return renderFunc;}
    std::function<void(int, int)> InputManager::getWindowResizeFunc() {return windowResizeFunc;}
    float InputManager::getMousePosX() {return mousePosX;}
    float InputManager::getMousePosY() {return mousePosY;}
    std::unordered_map<int/*GLFW_KEY*/, KeyStruct>& InputManager::getKeys() {return keyMap;}


    /***********
     * 
     * Setters
     * 
    ************/
    void InputManager::setGLFWWindow(GLFWwindow* window) 
    {
        InputManager::window = window;
        registerCallbacks();
    }
    void InputManager::setRenderFunc(std::function<void()> renderFunc) {InputManager::renderFunc = renderFunc;}
    void InputManager::setWindowResizeFunc(std::function<void(int, int)> windowResizeFunc) {InputManager::windowResizeFunc = windowResizeFunc;}
    void InputManager::setMousePosition(float x, float y)
    {
        mousePosX = x;
        mousePosY = y;
        if (window != nullptr)
            glfwSetCursorPos(window, mousePosX, mousePosY);
    }

    /***********
     * 
     * Input events
     * 
    ************/
    std::function<void()> InputManager::eventMouseLeftPress() {return funcMouseLeftPress;}
    std::function<void()> InputManager::eventMouseLeftRelease() {return funcMouseLeftRelease;}

    void InputManager::registerMouseLeftPress(std::function<void()> func) {funcMouseLeftPress = func;}
    void InputManager::registerMouseLeftRelease(std::function<void()> func) {funcMouseLeftRelease = func;}
    void InputManager::registerKey(std::function<void()> func, int key, InputType inputType, TriggerType triggerType) 
    {
        keyMap[key] = {
            func,
            key,
            inputType,
            triggerType
        };
    }
}