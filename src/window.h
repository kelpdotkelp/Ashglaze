#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "framebufferGeoSelect.h"

class Window
{
    private:
        GLFWwindow* window = nullptr;

        int width;
        int height;
        float deltaTime = 0.0f;
        float lastTime = 0.0f;

    public:
        Window(std::string name, int width, int height);
        Window();

        bool shouldClose();
        void swapBuffers();
        void closeWindow();
        void updateDeltaTime();

        int getWidth();
        int getHeight();
        float getAspectRatio();
        float getDeltaTime();
        GLFWwindow* getGLFWWindow();

        void setWidth(int width);
        void setHeight(int height);
};

#endif