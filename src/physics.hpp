#pragma once

#include <vector>

#include "Vector2.hpp"
#include "raylib.h"

#include "core.hpp"

namespace game::physics {
    struct Collider {
        std::vector<raylib::Vector2> points;
        raylib::Vector2 offset;
        Color debug_color = Fade(RED, 0.6);

        game::core::Entity* parent;

        public:
            Collider(game::core::Entity* parent, std::vector<raylib::Vector2> points);
            raylib::Vector2 collide_with_screen();
            bool collides_with(Collider& other);
    };
}
