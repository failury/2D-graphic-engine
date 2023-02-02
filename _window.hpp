#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
namespace my_engine{
class _window {
    public:
        _window(int w, int h, std::string name);
        ~_window();
        _window(const _window &) = delete; // prohibit copying
        _window &operator=(const _window &) = delete; // disable the operator
        bool shouldClose() {return glfwWindowShouldClose(window);}
        VkExtent2D getExtent() {return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};}
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);


    private:
        void initWindow();
        const int width;
        const int height;
        std::string windowName;
        GLFWwindow *window; //pointer to glfw window
};
}