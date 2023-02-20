#pragma once
#include <memory>
#include <vector>
#include "_window.hpp"
#include "engine_pipeline.hpp"
#include "_device.hpp"
#include "game_object.hpp"
namespace my_engine{
    class RenderSystem
    {
    
    public:
        RenderSystem(GameEngineDevice& device, VkRenderPass renderPass);
        ~RenderSystem();
        RenderSystem(const RenderSystem& ) = delete;
        RenderSystem &operator=(const RenderSystem&) = delete;

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects);


    private:
        void createPipelineLayout();
        void createPipeLine(VkRenderPass renderPass);
        


        GameEngineDevice &device;
        std::unique_ptr<Engine_Pipeline> engine_pipeline;
        VkPipelineLayout pipelineLayout;
    };
    
    
}