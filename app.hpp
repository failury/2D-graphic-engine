#pragma once
#include "_window.hpp"
#include "engine_pipeline.hpp"
namespace engine{
    class app
    {
    
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        void run();
    private:
        /* data */
        _window _window{WIDTH, HEIGHT, "My Name"};
        engine_pipeline enginepipeline{"shaders/simple_shader.vert.spv","shaders/simple_shader.frag.spv"};
    };
    
    
}