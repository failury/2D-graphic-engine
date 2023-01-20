#include "_window.hpp"
namespace engine
{
    _window::_window(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
        initWindow();
    }
    _window:: ~_window(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void _window:: initWindow(){
        glfwInit();
        // dont create opengl context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // dont resize after creating
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        // initialize the window pointer 
        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }
} // namespace engine
