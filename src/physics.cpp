#include "physics.hpp"
#include "Vector2.hpp"
#include "core.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace game::physics;

Collider::Collider(game::core::Entity* parent, std::vector<raylib::Vector2> points) {
    this->parent = parent;
    this->points = points;
}

raylib::Vector2 Collider::collide_with_screen() {
    raylib::Vector2 result;

    auto half_width = game::SCREEN_WIDTH / 2.;
    auto half_height = game::SCREEN_HEIGHT / 2.;

    for (auto p : points) {
        p = p.Rotate(parent->rotation);
        p += parent->position;
        p += offset;

        result.x += std::max(0.0, -half_width - p.x); // left wall
        result.x -= std::max(0.0, p.x - half_width); // right wall

        result.y += std::max(0.0, -half_height - p.y); // top wall
        result.y -= std::max(0.0, p.y - half_height); // bottom wall
    }

    return result;
}
