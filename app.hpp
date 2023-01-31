#pragma once
#include "_window.hpp"
#include "engine_pipeline.hpp"
#include "_device.hpp"
namespace my_engine{
    class app
    {
    
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        void run();
    private:
        /* data */
        _window _window{WIDTH, HEIGHT, "My Name"};
        GameEngineDevice GameEngineDevice{_window}; 
        engine_pipeline engine_pipeline{GameEngineDevice,"shaders/simple_shader.vert.spv","shaders/simple_shader.frag.spv",engine_pipeline::defaultPipeLineConfigInfo(WIDTH, HEIGHT)};
    };
    
    
}