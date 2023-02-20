#include "engine_renderer.hpp"

#include <stdexcept>
#include <array>
#include <iostream>

namespace my_engine
{
    EngineRenderer::EngineRenderer(_window &window, GameEngineDevice &device) : window{window}, device{device}
    {
        recreateSwapChain();
        createCommandBuffers();
    }

    EngineRenderer::~EngineRenderer()
    {
        freeCommandBuffers();
    }

    VkCommandBuffer EngineRenderer::beginFrame()
    {
        assert(!isFrameStarted && "Cannot call beginFrame while already in progress");
        auto result = engine_SwapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return nullptr;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to get swap chain image");
        }
        isFrameStarted = true;
        auto commandBuffer = getCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin create commandbuffer info");
        }
        return commandBuffer;
    }

    void EngineRenderer::endFrame()
    {
        assert(isFrameStarted && "Cannot call endFrame while frame is not in progress");
        auto commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer");
        }
        auto result = engine_SwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowsResized())
        {
            window.resetWindowResizedFlag();
            recreateSwapChain();
        }else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to  present swap image");
        }
        isFrameStarted = false;
    }

    void EngineRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Cannot call beginSwapChainRenderPass if frame is in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Cannot call beginSwapChainRenderPass on command buffer from a different frame");


        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = engine_SwapChain->getRenderPass();
        renderPassInfo.framebuffer = engine_SwapChain->getFrameBuffer(currentImageIndex);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = engine_SwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.1f, 0.1f, 0.1f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(engine_SwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(engine_SwapChain->getSwapChainExtent().height);
        viewport.maxDepth = 1.0f;
        viewport.minDepth = 0.0f;
        VkRect2D scissor{{0, 0}, engine_SwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void EngineRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Cannot call endSwapChainRenderPass if frame is in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Cannot call endSwapChainRenderPass on command buffer from a different frame");
        vkCmdEndRenderPass(commandBuffer);
    }

    void EngineRenderer::createCommandBuffers()
    {
        commandBuffers.resize(engine_SwapChain->imageCount());
        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandPool = device.getCommandPool();
        allocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        if (vkAllocateCommandBuffers(device.device(), &allocateInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers");
        };
    }

    void EngineRenderer::freeCommandBuffers()
    {
        vkFreeCommandBuffers(device.device(), device.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }

    void EngineRenderer::recreateSwapChain()
    {
        auto extent = window.getExtent();
        while (extent.width == 0 || extent.height == 0)
        {
            // when window minimized
            extent = window.getExtent();
            glfwWaitEvents(); // pause program
        }
        vkDeviceWaitIdle(device.device());
        if (engine_SwapChain == nullptr)
        {
            engine_SwapChain = std::make_unique<Engine_SwapChain>(device, extent);
        }
        else
        {
            engine_SwapChain = std::make_unique<Engine_SwapChain>(device, extent, std::move(engine_SwapChain));
            if (engine_SwapChain->imageCount() != commandBuffers.size())
            {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }
        // createPipeLine();
    }

}