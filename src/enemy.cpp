#include "enemy.hpp"
#include "director.hpp"
#include "Vector2.hpp"
#include "player.hpp"
#include "raylib.h"
#include "particles.hpp"
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

Enemy::Enemy(core::World& world) : core::Entity(world), body_collider(this, get_enemy_shape()) {}

void create_death_particles(game::core::World& world, raylib::Vector2 position, raylib::Vector2 velocity) {
    auto v_hat = velocity.Normalize();

    for (int idx = 0; idx < 6; idx++) {
        auto particle = std::make_shared<game::particles::Particle>(world, 1.0);

        particle->linear_velocity = v_hat.Rotate(randf() * PI / 10) * 1000 * Remap(randf(), 0, 1, 0.7, 1);
        particle->angular_velocity = Remap(randf(), 0, 1, 07 * PI, 10 * PI);
        particle->color = Fade(BLACK, 0.5);

        particle->linear_drag = 0.5;
        particle->angular_drag = 03;

        particle->position = position + (raylib::Vector2(1.).Rotate(randf() * 2 * PI) * randf() * 20.);

        particle->shape = game::particles::SQUARE;
        particle->radius = 15;

        world.add_entity(particle);
    }
}

void Enemy::update() {
    const float DESIRED_SPEED = 300;
    float lerp_speed = 0.01;
    if (velocity.Length() < DESIRED_SPEED) {
        lerp_speed = 0.05;
    }

    // TODO: use exp-decay
    velocity = velocity.Lerp((world.player->position - position).Normalize() * DESIRED_SPEED, lerp_speed);

    rotation = (velocity * raylib::Vector2(1, -1)).Angle(raylib::Vector2(1, 0));
    position += velocity * GetFrameTime();


    if (!is_on_screen()) return;

    bool going_fast = velocity.Length() > DESIRED_SPEED * 2;

    if (going_fast && particle_timer <= 0) {
        particle_timer = PARTICLE_TIMER;

        auto particle = std::make_shared<particles::Particle>(world, 0.35);
        particle->position = position;
        particle->color = Fade(BLACK, 0.5);
        particle->radius = 6;

        world.add_entity(particle);
    } else {
        particle_timer -= GetFrameTime();
    }

    for (auto e : world.get_entities()) {
        if (e.get() == this) continue;

        if (auto enemy = std::dynamic_pointer_cast<Enemy>(e)) {
            if (!enemy->is_on_screen()) continue;

            if (auto displacement = body_collider.collides_with(enemy->body_collider)) {
                position += displacement.value();

                bool other_going_fast = enemy->velocity.Length() > DESIRED_SPEED * 2;
                
                // TODO: Particles
                if (going_fast && other_going_fast) continue;
                if (other_going_fast) {
                    create_death_particles(world, position, enemy->velocity);
                    world.director->enemy_killed();
                    this->destroy();
                    break;
                }

                if (going_fast) {
                    create_death_particles(world, enemy->position, velocity);
                    world.director->enemy_killed();
                    enemy.get()->destroy();
                }

            }
        }
    }
}

void Enemy::draw() {
    if (!body_collider.is_on_screen()) return;

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
            Remap(sin(world.clock * 2 * PI), -1, 1, 0.3, 0.9)
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

bool Enemy::is_on_screen() {
    return body_collider.is_on_screen();
}
