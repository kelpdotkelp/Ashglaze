#include "window.h"

Window::Window()
{

}

Window::Window(std::string name, int width, int height)
{
    Window::width = width;
    Window::height = height;

    window = glfwCreateWindow(Window::width, Window::height, name.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    //Used to retrieve this instance in callbacks
    glfwSetWindowUserPointer(window, static_cast<void *>(this));

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        window = NULL;
    }
}

bool Window::shouldClose() {return glfwWindowShouldClose(window);}

void Window::closeWindow() {glfwSetWindowShouldClose(window, true);}

void Window::swapBuffers() {glfwSwapBuffers(window);}

void Window::setWidth(int width) {Window::width = width;}
void Window::setHeight(int height) {Window::height = height;}

int Window::getWidth() {return width;}
int Window::getHeight() {return height;}
float Window::getAspectRatio() {return ((float)width)/height;}
GLFWwindow* Window::getGLFWWindow() {return window;}
float Window::getDeltaTime() {return deltaTime;}

void Window::updateDeltaTime()
{
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}
