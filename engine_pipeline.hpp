#pragma once

#include "_device.hpp"
#include <vector>
#include <string>

namespace my_engine{
    struct PipeLineConfigInfo {

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
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
        Engine_Pipeline& operator=(const Engine_Pipeline&) = delete;
        static void defaultPipeLineConfigInfo(PipeLineConfigInfo &configinfo);
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