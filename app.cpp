#include "app.hpp"
namespace my_engine{
    void app::run(){
        while (!_window.shouldClose()){
            glfwPollEvents();
        }
    }
}