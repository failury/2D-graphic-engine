#include "app.hpp"
#include <stdexcept>
#include <array>
#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
namespace my_engine
{
    struct SimplePushConstantData{
        glm::mat2 transform{1.f};
        glm::vec2 offset;
        alignas(16)glm::vec3 color;
    };
    app::app()
    {
        loadGameObjects();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    app::~app()
    {
        vkDestroyPipelineLayout(GameEngineDevice.device(), pipelineLayout, nullptr);
    }

    void app::run()
    {
        while (!_window.shouldClose())
        {
            glfwPollEvents();
            drawFrame();
        }
        vkDeviceWaitIdle(GameEngineDevice.device());
    }
    void app::loadGameObjects()
    {
        std::vector<Model::Vertex> vertices{
            {{0.0f, -0.5f},{1.0f, 0.0f, 0.0f}}, {{0.5f, 0.5f},{0.0f, 1.0f, 0.0f}}, {{-0.5f, 0.5f},{0.0f, 0.0f, 1.0f}}};
        auto model = std::make_shared<Model>(GameEngineDevice, vertices);
        auto triangle = GameObject::createGameObject();
        triangle.model = model;
        triangle.color = {.1f, .8f, .1f};
        triangle.transform2d.translation.x = .2f;
        triangle.transform2d.scale = {2.f, .5f};
        triangle.transform2d.rotation = .25f * glm:: two_pi<float>();
        gameObjects.push_back(std::move(triangle));
    }
    void app::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange {};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(GameEngineDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout");
        }
    }
    void app::createPipeLine()
    {
        assert(engine_SwapChain != nullptr && "Cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
        PipeLineConfigInfo pipelineConfig {};
        Engine_Pipeline::defaultPipeLineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = engine_SwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        engine_pipeline = std::make_unique<Engine_Pipeline>(GameEngineDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
    }
    void app::createCommandBuffers()
    {
        commandBuffers.resize(engine_SwapChain->imageCount());
        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandPool = GameEngineDevice.getCommandPool();
        allocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        if (vkAllocateCommandBuffers(GameEngineDevice.device(), &allocateInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers");
        };

        
    }

    void app::freeCommandBuffers()
    {
        vkFreeCommandBuffers(GameEngineDevice.device(), GameEngineDevice.getCommandPool(), static_cast<uint32_t> (commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }

    void app::drawFrame()
    {
        uint32_t imageIndex;
        auto result = engine_SwapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR){
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to get swap chain image");
        }
        recordCommandBuffer(imageIndex);
        result = engine_SwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window.wasWindowsResized()){
            _window.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to  present swap image");
        }
    }
    void app::recreateSwapChain()
    {
        auto extent = _window.getExtent();
        while (extent.width == 0 || extent.height == 0){
            // when window minimized
            extent =_window.getExtent();
            glfwWaitEvents(); // pause program
        }
        vkDeviceWaitIdle(GameEngineDevice.device());
        if (engine_SwapChain == nullptr){
            engine_SwapChain = std:: make_unique<Engine_SwapChain>(GameEngineDevice, extent);
        }else{
            engine_SwapChain = std:: make_unique<Engine_SwapChain>(GameEngineDevice, extent, std::move(engine_SwapChain));
            if (engine_SwapChain->imageCount() != commandBuffers.size()){
                freeCommandBuffers();
                createCommandBuffers();
            }
        }
        createPipeLine();

    }
    void app::recordCommandBuffer(int imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to begin create commandbuffer info");
            }
            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = engine_SwapChain->getRenderPass();
            renderPassInfo.framebuffer = engine_SwapChain->getFrameBuffer(imageIndex);
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = engine_SwapChain->getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.01f, 0.1f, 0.1f, 0.1f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(engine_SwapChain->getSwapChainExtent().width);
            viewport.height = static_cast<float>(engine_SwapChain->getSwapChainExtent().height);
            viewport.maxDepth = 1.0f;
            viewport.minDepth = 0.0f;
            VkRect2D scissor{{0,0}, engine_SwapChain->getSwapChainExtent()};
            vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
            vkCmdSetScissor(commandBuffers[imageIndex], 0,1, &scissor);
            renderGameObjects(commandBuffers[imageIndex]);
            vkCmdEndRenderPass(commandBuffers[imageIndex]);
            if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to record command buffer");
            }
    }
    void app::renderGameObjects(VkCommandBuffer commandBuffer)
    {
        engine_pipeline->Bind(commandBuffer);
        for (auto & obj: gameObjects){
            obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm:: two_pi<float>());
            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            push.transform = obj.transform2d.mat2();
            vkCmdPushConstants(commandBuffer, pipelineLayout,VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,0, sizeof(SimplePushConstantData),&push);
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }

    }
}