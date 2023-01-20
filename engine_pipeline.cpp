#include "engine_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
namespace engine{
    engine_pipeline::engine_pipeline(const std::string &vertFilePath, const std::string &fragFilePath)
    {
        createPipeline(vertFilePath, fragFilePath);
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
    void engine_pipeline::createPipeline(const std::string &vertFilePath, const std::string &fragFilePath)
    {
        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFilePath);

        std::cout << "vsize" << vertCode.size() << "\n";
        std::cout << "fsize" << fragCode.size() << "\n";
    }
}