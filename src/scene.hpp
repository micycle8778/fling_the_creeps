#pragma once

#include "core.hpp"

namespace game::scene {
    const float SPAWN_TIMER = 2.0;

    class FPSLabel : public core::Entity {
        public:
            FPSLabel();
            void update(core::World& world) {}
            void draw(core::World& world);
    };

    class Scene : public core::Entity {
        float spawn_timer = 0;

        public:
            Scene(core::World& world);
            void update(core::World& world);
            void draw(core::World& world) {}
    };
}
