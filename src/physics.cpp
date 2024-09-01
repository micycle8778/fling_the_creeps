#include "physics.hpp"
#include "Vector2.hpp"
#include "core.hpp"
#include <algorithm>
#include <cmath>
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

bool Collider::collides_with(Collider& other) {
    auto our_points = get_transformed_points();
    auto other_points = other.get_transformed_points();

    // Do collision check or whatever
    for (auto container : {our_points, other_points}) {
        for (auto it = container.begin(); it != container.end(); it++) {
            for (auto jt = it + 1; jt != container.end(); jt++) {
                auto v = *it - *jt;
                auto axis = raylib::Vector2(v.y, -v.x);

                float our_left = INFINITY;
                float our_right = -INFINITY;

                float other_left = INFINITY;
                float other_right = -INFINITY;

                for (auto p : our_points) {
                    auto dot = p.DotProduct(axis);

                    our_left = std::min(our_left, dot);
                    our_right = std::max(our_right, dot);
                }

                for (auto p : other_points) {
                    auto dot = p.DotProduct(axis);

                    other_left = std::min(other_left, dot);
                    other_right = std::max(other_right, dot);
                }

                if (!(our_right >= other_left && other_right >= our_left)) {
                    return false;
                }
            }
        }
    }

    return true;
}

std::vector<raylib::Vector2> Collider::get_transformed_points() {
    auto result = points;

    for (auto& p : result) {
        p += offset;
        p = p.Rotate(parent->rotation);
        p += parent->position;
    }

    return result;
}
