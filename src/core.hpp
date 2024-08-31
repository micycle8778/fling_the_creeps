#pragma once

#include <memory>
#include <vector>

#include "Vector2.hpp"

namespace game::physics {
    struct Collider;
}

namespace game {
    const float SCREEN_WIDTH = 1280;
    const float SCREEN_HEIGHT = 720;

    namespace core {
        class Entity : public std::enable_shared_from_this<Entity> {
            public:
                raylib::Vector2 position;
                float rotation;

                std::vector<physics::Collider> colliders;

                virtual void update() = 0;
                virtual void draw() = 0;
        };

        class World {
            std::vector<std::shared_ptr<Entity>> entities;

            public:
                void add_entity(std::shared_ptr<Entity> entity);
                void update();
        };
    }
}
