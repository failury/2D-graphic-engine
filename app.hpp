#pragma once
#include <memory>
#include <vector>
#include "_window.hpp"
#include "engine_pipeline.hpp"
#include "_device.hpp"
#include "engine_swapchain.hpp"
#include "vertex_model.hpp"
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
        void loadModel();
        void createPipelineLayout();
        void createPipeLine();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);
        /* data */
        _window _window{WIDTH, HEIGHT, "My Name"};
        GameEngineDevice GameEngineDevice{_window};
        std:: unique_ptr<Engine_SwapChain> engine_SwapChain;
        std::unique_ptr<Engine_Pipeline> engine_pipeline;
        VkPipelineLayout pipelineLayout;
        std:: vector<VkCommandBuffer> commandBuffers;
        std:: unique_ptr<Model> model;
    };
    
    
}