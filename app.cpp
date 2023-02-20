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
    struct SimplePushConstantData
    {
        glm::mat2 transform{1.f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };
    app::app()
    {
        loadGameObjects();
        createPipelineLayout();
        createPipeLine();
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
            if (auto commandBuffer = EngineRenderer.beginFrame()){
                // beginFrame will return null if failed
                EngineRenderer.beginSwapChainRenderPass(commandBuffer);
                renderGameObjects(commandBuffer);
                EngineRenderer.endSwapChainRenderPass(commandBuffer);
                EngineRenderer.endFrame();
            }
        }
        vkDeviceWaitIdle(GameEngineDevice.device());
    }
    void app::loadGameObjects()
    {
        std::vector<Model::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  auto lveModel = std::make_shared<Model>(GameEngineDevice, vertices);

  auto triangle = GameObject::createGameObject();
  triangle.model = lveModel;
  triangle.color = {.1f, .8f, .1f};
  triangle.transform2d.translation.x = .2f;
  triangle.transform2d.scale = {2.f, .5f};
  triangle.transform2d.rotation = .25f * glm::two_pi<float>();

  gameObjects.push_back(std::move(triangle));
    }
    void app::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
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
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
        PipeLineConfigInfo pipelineConfig{};
        Engine_Pipeline::defaultPipeLineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = EngineRenderer.getSwapChainRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        engine_pipeline = std::make_unique<Engine_Pipeline>(GameEngineDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
    }



    
void app::renderGameObjects(VkCommandBuffer commandBuffer)
    {

        engine_pipeline->Bind(commandBuffer);
        for (auto &obj : gameObjects)
        {
            obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());
            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            push.transform = obj.transform2d.mat2();
            vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }
}