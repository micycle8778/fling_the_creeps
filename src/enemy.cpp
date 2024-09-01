#include "enemy.hpp"
#include "Vector2.hpp"
#include "player.hpp"
#include "raylib.h"
#include <cmath>
#include <memory>

using namespace game::enemy;

std::vector<raylib::Vector2> get_enemy_shape() {
    return std::vector<raylib::Vector2>({
        raylib::Vector2(50, 0),
        raylib::Vector2(-50, -45),
        raylib::Vector2(-50, 45)
    });
}

Enemy::Enemy() : body_collider(this, get_enemy_shape()) {}

void Enemy::update(core::World& world) {
    const float DESIRED_SPEED = 300;
    float lerp_speed = 0.01;
    if (velocity.Length() < DESIRED_SPEED) {
        lerp_speed = 0.05;
    }
    // TODO:
    velocity = velocity.Lerp((world.player->position - position).Normalize() * DESIRED_SPEED, lerp_speed);

    rotation = (velocity * raylib::Vector2(1, -1)).Angle(raylib::Vector2(1, 0));
    position += velocity * GetFrameTime();


    bool going_fast = velocity.Length() > DESIRED_SPEED * 2;

    for (auto e : world.get_entities()) {
        if (e.get() == this) continue;

        if (auto enemy = std::dynamic_pointer_cast<Enemy>(e)) {
            if (auto displacement = body_collider.collides_with(enemy->body_collider)) {
                position += displacement.value();

                bool other_going_fast = enemy->velocity.Length() > DESIRED_SPEED * 2;
                
                // TODO: Particles
                if (going_fast && other_going_fast) continue;
                if (other_going_fast) {
                    world.destroy(this);
                    break;
                }

                if (going_fast) {
                    world.destroy(enemy.get());
                }

            }
        }
    }
}

void Enemy::draw(core::World& world) {
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
            Remap(sin(world.clock * 2 * PI), -1, 1, 0.2, 0.8)
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
