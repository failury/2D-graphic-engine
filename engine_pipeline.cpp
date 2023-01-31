#include "engine_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cassert>
namespace my_engine{

    engine_pipeline::engine_pipeline(GameEngineDevice &device, const std::string &vertFilePath, const std::string &fragFilePath, const PipeLineConfigInfo &config) : device{device}
    {
        createPipeline(vertFilePath, fragFilePath, config);
    }

    engine_pipeline::engine_pipeline::~engine_pipeline()
    {
        vkDestroyShaderModule(device.device(), vertexShaderModule, nullptr);
        vkDestroyShaderModule(device.device(), fragShaderModule, nullptr);
        vkDestroyPipeline(device.device(), graphicsPipeline, nullptr);

    }

    PipeLineConfigInfo engine_pipeline::defaultPipeLineConfigInfo(uint32_t width, uint32_t height)
    {
        PipeLineConfigInfo configinfo{};
        // specify the input assemblytypes
        configinfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configinfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configinfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        // viewports, screen width, height, depth
        // how we transfer the input to output image
        configinfo.viewport.x = 0.0f;
        configinfo.viewport.y = 0.0f;
        configinfo.viewport.width = static_cast<float>(width);
        configinfo.viewport.height = static_cast<float>(height);
        configinfo.viewport.maxDepth = 1.0f;
        configinfo.viewport.minDepth = 0.0f;

        configinfo.scissor.offset = {0,0};
        configinfo.scissor.extent = {width, height};

        configinfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        configinfo.viewportInfo.viewportCount = 1;
        configinfo.viewportInfo.pViewports = &configinfo.viewport;
        configinfo.viewportInfo.scissorCount = 1;
        configinfo.viewportInfo.pScissors = &configinfo.scissor;

        // rasterization stages
        configinfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configinfo.rasterizationInfo.depthBiasClamp = VK_FALSE;
        configinfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configinfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        // vertex order
        configinfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
        configinfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configinfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
        configinfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
        configinfo.rasterizationInfo.depthBiasClamp = 0.0f;
        configinfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

        // multisampling
        configinfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configinfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
        configinfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configinfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
        configinfo.multisampleInfo.pSampleMask = nullptr;             // Optional
        configinfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
        configinfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

        // color blending: how we combine colors in frame buffer
        configinfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        configinfo.colorBlendAttachment.blendEnable = VK_FALSE;
        configinfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configinfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configinfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
        configinfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configinfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configinfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional
        
        configinfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configinfo.colorBlendInfo.logicOpEnable = VK_FALSE;
        configinfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
        configinfo.colorBlendInfo.attachmentCount = 1;
        configinfo.colorBlendInfo.pAttachments = &configinfo.colorBlendAttachment;
        configinfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
        configinfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
        configinfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
        configinfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional
        
        configinfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configinfo.depthStencilInfo.depthTestEnable = VK_TRUE;
        configinfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
        configinfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configinfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configinfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
        configinfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
        configinfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
        configinfo.depthStencilInfo.front = {};  // Optional
        configinfo.depthStencilInfo.back = {};   // Optional



        return configinfo;
    }

    std::vector<char> engine_pipeline::readFile(const std::string &filepath)
    {
        // seek to end when reading
        std:: ifstream file(filepath, std::ios::ate | std::ios::binary); 
        if (!file.is_open()){
            throw std::runtime_error("Failed to open file: " + filepath);
        }
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);
        file.seekg(0); //seek to start
        file.read(buffer.data(), fileSize); // copy to buffer
        file.close();
        return buffer;
    }
    void engine_pipeline::createPipeline(const std::string & vertFilePath, const std::string & fragFilePath, const PipeLineConfigInfo &config)
    {
        assert(config.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline: no pipelinelayout provided in config info");
        assert(config.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline: no renderpass provided in config info");

        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFilePath);

        createShaderModule(vertCode, &vertexShaderModule);
        createShaderModule(fragCode, &fragShaderModule);

        VkPipelineShaderStageCreateInfo shaderStages[2];
        // 0 for vertex 1 for frag
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = vertexShaderModule;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;

        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = fragShaderModule;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;

        VkGraphicsPipelineCreateInfo pipelineInfo {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &config.inputAssemblyInfo;
        pipelineInfo.pViewportState = &config.viewportInfo;
        pipelineInfo.pRasterizationState = &config.rasterizationInfo;
        pipelineInfo.pMultisampleState = &config.multisampleInfo;
        pipelineInfo.pColorBlendState = &config.colorBlendInfo;
        pipelineInfo.pDepthStencilState = &config.depthStencilInfo;
        pipelineInfo.pDynamicState = nullptr;

        pipelineInfo.layout = config.pipelineLayout;
        pipelineInfo.renderPass = config.renderPass;
        pipelineInfo.subpass = config.subpass;

        pipelineInfo.basePipelineIndex = -1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS){
            throw std:: runtime_error("failed to create Graphic Pipeline");
        }



    }

    void engine_pipeline::createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*> (code.data());

        if (vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS){
            throw std:: runtime_error("failed to create shader module");
        }
    }
}