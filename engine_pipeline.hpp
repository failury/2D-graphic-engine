#pragma once

#include <vector>
#include <string>

namespace engine{
    class engine_pipeline
    {
    public:
        engine_pipeline(const std::string & vertFilePath, const std::string & fragFilePath);
    private:
        /* data */
        static std::vector<char> readFile(const std:: string& filepath);
        void createPipeline(const std::string & vertFilePath, const std::string & fragFilePath);
    

    };
}