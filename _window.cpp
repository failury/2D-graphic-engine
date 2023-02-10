#include "_window.hpp"
#include <stdexcept>
namespace my_engine
{
    _window::_window(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
        initWindow();
    }
    _window:: ~_window(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void _window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
            throw std::runtime_error("failed to create window surface");
        }
    }
    void _window::framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        // reinterpret_cast does not garrantee the address of the value staying the same
        auto w =  reinterpret_cast<_window *>(glfwGetWindowUserPointer(window));
        w->frameBufferResized = true;
        w->width = width;
        w->height = height;
    }
    void _window::initWindow()
    {
        glfwInit();
        // dont create opengl context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // dont resize after creating
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        // initialize the window pointer 
        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }
} // namespace engine
