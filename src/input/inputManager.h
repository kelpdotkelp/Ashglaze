#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <functional>
#include <string>
#include <unordered_map>

#include "callbacks.h"
#include "key.h"

namespace input
{
    class InputManager
    {
        private:
            static void eventNotBound() {}

            GLFWwindow* window = nullptr;
            std::function<void(int, int)> windowResizeFunc = nullptr;
            std::function<void()> renderFunc = nullptr;

            //Polled value, continually updated
            float mousePosX;
            float mousePosY;

            std::function<void()> funcMouseLeftPress = &eventNotBound;
            std::function<void()> funcMouseLeftRelease = &eventNotBound;

            std::unordered_map<int/*GLFW_KEY*/, KeyStruct> keyMap;

            void registerCallbacks();

        public:

            InputManager();

            //Getters
            GLFWwindow* getGLFWWindow();
            std::function<void()> getRenderFunc();
            std::function<void(int, int)> getWindowResizeFunc();
            float getMousePosX();
            float getMousePosY();
            std::unordered_map<int/*GLFW_KEY*/, KeyStruct>& getKeys();

            //Setters
            void setGLFWWindow(GLFWwindow* window);
            void setRenderFunc(std::function<void()> renderFunc);
            void setWindowResizeFunc(std::function<void(int, int)> windowResizeFunc);
            void setMousePosition(float x, float y);

            //Polling
            void pollInputs();

            std::function<void()> eventMouseLeftPress();
            std::function<void()> eventMouseLeftRelease();

            //Register inputs
            void registerMouseLeftPress(std::function<void()> func);
            void registerMouseLeftRelease(std::function<void()> func);

            void registerKey(std::function<void()> func, int key, InputType inputType, TriggerType triggerType);
    };
}

#endif