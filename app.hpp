#pragma once
#include <memory>
#include <vector>
#include "_window.hpp"
#include "engine_pipeline.hpp"
#include "_device.hpp"
#include "engine_renderer.hpp"
#include "game_object.hpp"
namespace my_engine{
    class app
    {
    
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        app();
        ~app();
        app(const app& ) = delete;
        app &operator=(const app&) = delete;
        void run();


    private:
        void loadGameObjects();
        void createPipelineLayout();
        void createPipeLine();
        void renderGameObjects(VkCommandBuffer commandBuffer);


        _window _window{WIDTH, HEIGHT, "My Name"};
        GameEngineDevice GameEngineDevice{_window};
        EngineRenderer EngineRenderer{_window, GameEngineDevice};
        std::unique_ptr<Engine_Pipeline> engine_pipeline;
        VkPipelineLayout pipelineLayout;
        std:: vector<GameObject> gameObjects;
    };
    
    
}