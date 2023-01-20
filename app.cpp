#include "app.hpp"
namespace engine{
    void app::run(){
        while (!_window.shouldClose()){
            glfwPollEvents();
        }
    }
}