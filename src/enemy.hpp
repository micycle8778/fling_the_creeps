#pragma once

#include "core.hpp"
#include "physics.hpp"

namespace game::enemy {
    class Enemy : public core::Entity {
        public:
            raylib::Vector2 velocity;
            physics::Collider body_collider;

            Enemy();
            void update(core::World& world);
            void draw(core::World& world);
    };
}
