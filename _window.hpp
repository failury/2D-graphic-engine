#pragma once
#include <GLFW/glfw3.h>
#include <string>
#define GLFW_INCLUDE_VULKAN
namespace engine{
class _window {
    public:
        _window(int w, int h, std::string name);
        ~_window();
        _window(const _window &) = delete; // prohibit copying
        _window &operator=(const _window &) = delete; // disable the operator
        bool shouldClose() {return glfwWindowShouldClose(window);}


    private:
        void initWindow();
        const int width;
        const int height;
        std::string windowName;
        GLFWwindow *window; //pointer to glfw window
};
}