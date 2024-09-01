#include "enemy.hpp"
#include "Color.hpp"
#include "Vector2.hpp"
#include <cmath>

using namespace game::enemy;

std::vector<raylib::Vector2> get_enemy_shape() {
    return std::vector<raylib::Vector2>({
        raylib::Vector2(50, 0),
        raylib::Vector2(-50, -45),
        raylib::Vector2(-50, 45)
    });
}

Enemy::Enemy() : body_collider(this, get_enemy_shape()) {}

void Enemy::update(core::World& _world) {}

void Enemy::draw(core::World& _world) {
    { // draw body
        auto points = get_enemy_shape();
        points.insert(std::begin(points), raylib::Vector2());
        points.push_back(points[1]);

        for (auto& p : points) {
            p = p.Rotate(rotation);
            p += position;
        }

        DrawTriangleFan(points.data(), points.size(), BLACK);
    }

    { // draw eyes
        auto eye_brightness = Fade(RED,
            Remap(sin(_world.clock * 2 * PI), -1, 1, 0.2, 0.8)
        );

        std::vector<raylib::Vector2> eyes({
            raylib::Vector2(0, 10),
            raylib::Vector2(0, -10)
        });

        for (auto p : eyes) {
            p = p.Rotate(rotation);
            p += position;

            DrawCircleV(p, 5, eye_brightness);
        }
    }
}
