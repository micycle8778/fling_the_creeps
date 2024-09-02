#pragma once

#include <optional>
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

        float radius = 0;

        Collider(const Collider&) = delete;
        Collider& operator=(const Collider&) = delete;

        public:
            Collider(game::core::Entity* parent, std::vector<raylib::Vector2> points);
            std::optional<raylib::Vector2> collide_with_screen();
            std::optional<raylib::Vector2> collides_with(Collider& other);
            std::vector<raylib::Vector2> get_transformed_points();
            bool is_on_screen();
    };
}
