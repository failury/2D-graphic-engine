#pragma once

#include "_device.hpp"
#include <vector>
#include <string>

namespace my_engine{
    struct PipeLineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportInfo;
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
    class engine_pipeline
    {
    public:
        engine_pipeline(
      GameEngineDevice& device,
      const std::string& vertFilepath,
      const std::string& fragFilepath,
      const PipeLineConfigInfo& configInfo);
      ~engine_pipeline();
        // avoid copying
        engine_pipeline(const engine_pipeline&) = delete;
        void operator=(const engine_pipeline&) = delete;
        static PipeLineConfigInfo defaultPipeLineConfigInfo(uint32_t width, uint32_t height);

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