#include "callbacks.h"

namespace input
{
    void framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        InputManager* im = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
        if (im->getWindowResizeFunc() != nullptr)
            (im->getWindowResizeFunc())(width, height);
        glViewport(0, 0, width, height);
    }

    void windowRefreshCallback(GLFWwindow* window)
    {
        InputManager* im = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
        if (im->getRenderFunc() != nullptr)
        {
            (im->getRenderFunc())();
            glfwSwapBuffers(im->getGLFWWindow());
            glFinish();
        }
    }

    void mousePositionCallback(GLFWwindow* window, double xPos, double yPos)
    {
        InputManager* im = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
        im->setMousePosition(xPos, yPos);
    }

    void mousePressedCallback(GLFWwindow* window, int button, int action, int mods)
    {
        InputManager* im = static_cast<InputManager *>(glfwGetWindowUserPointer(window));

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            (im->eventMouseLeftPress())();
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
            (im->eventMouseLeftRelease())();
    }

    void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        InputManager* im = static_cast<InputManager *>(glfwGetWindowUserPointer(window));

        for (auto& it: im->getKeys())
        {
            if (it.second.inputType == input::InputType::EVENT &&
                it.second.key == key)
            {
                if (it.second.triggerType == input::TriggerType::CONTINUOUS)
                    (it.second.keyBindFunc)();
                else if (it.second.triggerType == input::TriggerType::SINGLE_PRESS)
                {
                    if (action == GLFW_PRESS && it.second.isSinglePressReady)
                    {
                        (it.second.keyBindFunc)();
                        it.second.isSinglePressReady = false;
                    }
                    if (action == GLFW_RELEASE)
                    {
                        it.second.isSinglePressReady = true;
                    }
                }

                break;
            }
        }
    }
}