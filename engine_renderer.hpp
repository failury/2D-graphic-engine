#pragma once
#include <memory>
#include <vector>
#include <cassert>
#include "_window.hpp"
#include "engine_pipeline.hpp"
#include "_device.hpp"
#include "engine_swapchain.hpp"
#include "game_object.hpp"
namespace my_engine{
    class EngineRenderer
    {
    
    public:
        EngineRenderer(_window& window, GameEngineDevice& device);
        ~EngineRenderer();
        EngineRenderer(const EngineRenderer& ) = delete;
        EngineRenderer &operator=(const EngineRenderer&) = delete;
        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
        bool isFrameInProgress() const {return isFrameStarted;}

        VkCommandBuffer getCurrentCommandBuffer() const 
        {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentImageIndex];
            }
        VkRenderPass getSwapChainRenderPass() const{
            return engine_SwapChain->getRenderPass();
        }


    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        /* data */
        _window& window;
        GameEngineDevice& device;
        std:: unique_ptr<Engine_SwapChain> engine_SwapChain;
        std:: vector<VkCommandBuffer> commandBuffers;
        uint32_t currentImageIndex;
        bool isFrameStarted{false};
    };
    
    
}