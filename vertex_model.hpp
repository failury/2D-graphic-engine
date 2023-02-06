#pragma once

#include "_device.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <vector>
#include <glm/glm.hpp>
namespace my_engine {
    class Model{
        public:
            struct Vertex{
                glm::vec2 position;
                static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
                static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
            };
            Model(GameEngineDevice &device, const std::vector<Vertex> &vertices);
            ~Model();
            Model(const Model& ) = delete;
            Model &operator=(const Model&) = delete;
            void bind(VkCommandBuffer commandBuffer);
            void draw(VkCommandBuffer commandBuffer);
        private:
            GameEngineDevice &engineDevice;
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            uint32_t vertexCount;
            void createVertexBuffers(const std::vector<Vertex> &vertices);

    };
}