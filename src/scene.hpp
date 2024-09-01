#pragma once

#include "core.hpp"

namespace game::scene {
    const float SPAWN_TIMER = 2.0;

    class Scene : public core::Entity {
        float spawn_timer = 0;

        public:
            Scene();
            void update(core::World& world);
            void draw(core::World& world) {}
    };
}
