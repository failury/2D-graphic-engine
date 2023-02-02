#pragma once

#include "_device.hpp"
#include <vector>
#include <string>

namespace my_engine{
    struct PipeLineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };
    class Engine_Pipeline
    {
    public:
        Engine_Pipeline(
      GameEngineDevice& device,
      const std::string& vertFilepath,
      const std::string& fragFilepath,
      const PipeLineConfigInfo& configInfo);
      ~Engine_Pipeline();
        // avoid copying
        Engine_Pipeline(const Engine_Pipeline&) = delete;
        void operator=(const Engine_Pipeline&) = delete;
        static PipeLineConfigInfo defaultPipeLineConfigInfo(uint32_t width, uint32_t height);
        void Bind(VkCommandBuffer commandBuffer);

    private:
        /* data */
        static std::vector<char> readFile(const std:: string& filepath);
        void createPipeline(const std::string & vertFilePath, const std::string & fragFilePath, const PipeLineConfigInfo &config);
        void createShaderModule(const std::vector<char> & code, VkShaderModule* shaderModule);
        GameEngineDevice &device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertexShaderModule;
        VkShaderModule fragShaderModule;
    };
}