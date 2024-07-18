#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "inputManager.h"

namespace input
{
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void windowRefreshCallback(GLFWwindow* window);
    void mousePositionCallback(GLFWwindow* window, double xPos, double yPos);
    void mousePressedCallback(GLFWwindow* window, int button, int action, int mods);
    void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
}

#endif