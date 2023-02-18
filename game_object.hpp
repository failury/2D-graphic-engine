#pragma once

#include "vertex_model.hpp"

#include <memory>
namespace my_engine
{
    struct Transform2dComponent {
        glm:: vec2 translation{};
        glm:: vec2 scale{.1f, .1f};
        float rotation;
        glm::mat2 mat2() 
        {
            const float s = glm:: sin(rotation);
            const float c = glm:: cos(rotation);
            glm:: mat2 rotMat {{c, s}, {-s, c}};
            glm:: mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
            return rotMat * scaleMat;
            }

    };
    class GameObject {
    public:
        using id_t = unsigned int; // define id type
        static GameObject createGameObject(){
            static id_t currentId = 0;
            return GameObject{currentId++};
        }
        id_t getId(){return id;}
        GameObject(const GameObject &) = delete;
        GameObject &operator=(const GameObject &) = delete;
        GameObject(GameObject&&) = default;
        GameObject &operator=(GameObject&&);

        std::shared_ptr<Model> model{};
        glm::vec3 color{};
        Transform2dComponent transform2d{};

    private:
        GameObject(id_t objId): id{objId} {}
        id_t id;
        
    };

};
// namespace my_engine
