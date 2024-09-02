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

    parent->colliders.push_back(this);

    for (auto p : points) {
        auto new_radius = (p + offset).Length();
        if (new_radius > radius) {
            radius = new_radius;
        }
    }
}

std::optional<raylib::Vector2> Collider::collide_with_screen() {
    raylib::Vector2 result;

    auto half_width = game::SCREEN_WIDTH / 2.;
    auto half_height = game::SCREEN_HEIGHT / 2.;

    for (auto p : get_transformed_points()) {
        result.x += std::max(0.0, -half_width - p.x); // left wall
        result.x -= std::max(0.0, p.x - half_width); // right wall

        result.y += std::max(0.0, -half_height - p.y); // top wall
        result.y -= std::max(0.0, p.y - half_height); // bottom wall
    }

    if (result == raylib::Vector2()) 
        return {};
    return result;
}

std::optional<raylib::Vector2> Collider::collides_with(Collider& other) {
    // broad circle-to-circle
    if (!CheckCollisionCircles(
                parent->position, radius, 
                other.parent->position, other.radius
    )) return {};

    // SAT
    auto our_points = get_transformed_points();
    auto other_points = other.get_transformed_points();

    raylib::Vector2 displacement = INFINITY;
    for (auto container : {our_points, other_points}) {
        for (auto it = container.begin(); it != container.end(); it++) {
            auto p1 = *it;
            auto p2 = it != container.end() - 1 ? *(it + 1) : *container.begin();

            auto v = p1 - p2;
            auto axis = raylib::Vector2(v.y, -v.x).Normalize();

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
                return {};
            }

            raylib::Vector2 potential_displacement;
            if (our_left < other_left) {
                potential_displacement = axis * (other_left - our_right - 1);
            } else {
                potential_displacement = axis * (other_right - our_left - 1);
            }

            if (potential_displacement.Length() < displacement.Length()) {
                displacement = potential_displacement;
            }
        }
    }

    return displacement;
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

bool Collider::is_on_screen() {
    auto half_width = game::SCREEN_WIDTH / 2.;
    auto half_height = game::SCREEN_HEIGHT / 2.;

    for (auto p : get_transformed_points()) {
        if (p.x >= -half_width && p.x <= half_width) {
            if (p.y >= -half_height && p.y <= half_height) {
                return true;
            }
        }
    }

    return false;
}
