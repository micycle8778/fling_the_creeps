#pragma once

#include "core.hpp"
#include "physics.hpp"

namespace game::enemy {
    const float PARTICLE_TIMER = 0.025;

    class Enemy : public core::Entity {
        float particle_timer = 0;
        
        public:
            raylib::Vector2 velocity;
            physics::Collider body_collider;

            Enemy();
            void update(core::World& world);
            void draw(core::World& world);
            bool is_on_screen();
    };
}
