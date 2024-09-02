#include "Color.hpp"
#include "Vector2.hpp"
#include "player.hpp"
#include "core.hpp"
#include "physics.hpp"
#include "enemy.hpp"

#include "raylib.h"
#include <algorithm>
#include <cstdlib>
#include <memory>

using namespace game::player;

const float ACCELERATION = 6000.;
const float MAX_SPEED = 1500.;
const int PLAYER_SIDES = 8;

float angle_difference(float from, float to) {
    float difference = fmod(to - from, PI * 2);
    return fmod(2.0 * difference, PI * 2) - difference;
}

float lerp_angle(float from, float to, float t) {
    return from + angle_difference(from, to) * t;
}

std::vector<raylib::Vector2> create_player_shape(int points) {
    std::vector<raylib::Vector2> result;

    for (int i = 0; i < points; i++) {
        float theta = ((PI * 2) / points) * -i;
        raylib::Vector2 point(cos(theta), sin(theta));

        result.push_back(point * 35.);
    }

    return result;
}

std::vector<raylib::Vector2> create_hammer_head_shape(float scale = 1) {
    const raylib::Vector2 HAMMER_HEAD_HALF_SIZE(50, 30);

    return std::vector<raylib::Vector2>({
        HAMMER_HEAD_HALF_SIZE * raylib::Vector2(1, 1) * scale,
        HAMMER_HEAD_HALF_SIZE * raylib::Vector2(1, -1) * scale,
        HAMMER_HEAD_HALF_SIZE * raylib::Vector2(-1, -1) * scale,
        HAMMER_HEAD_HALF_SIZE * raylib::Vector2(-1, 1) * scale,
    });
}

Player::Player(core::World& world) : 
    player_collider(this, create_player_shape(PLAYER_SIDES)),
    hammer_collider(this, create_hammer_head_shape(1.55))
{
    hammer_collider.offset = raylib::Vector2(0, 125);
}

void Player::_move_player() {
    raylib::Vector2 input;

    if (IsKeyDown(KEY_A)) {
        input.x -= 1;
    }

    if (IsKeyDown(KEY_D)) {
        input.x += 1;
    }

    if (IsKeyDown(KEY_W)) {
        input.y -= 1;
    }

    if (IsKeyDown(KEY_S)) {
        input.y += 1;
    }

    velocity += input.Normalize() * ACCELERATION * GetFrameTime();
    
    if (velocity.Length() > MAX_SPEED) {
        velocity = velocity.Normalize() * MAX_SPEED;
    }

    velocity -= velocity * 5 * GetFrameTime();

    position += velocity * GetFrameTime();

    if (input != raylib::Vector2() && state == READY) { // TODO: use exp decay instead
        float desired_angle = (input * raylib::Vector2(1, -1)).Angle(raylib::Vector2(0., 1.));
        rotation = lerp_angle(rotation, desired_angle, 0.025);
    }

}

const float SWING_TIME = 0.15;
const float RECOVERY_TIME = 0.50;

void Player::_handle_hammer_collision(core::World& world) {
    for (auto e : world.get_entities()) {
        if (auto enemy = std::dynamic_pointer_cast<enemy::Enemy>(e)) {
            if (!enemy->is_on_screen()) continue;

            if (hit_enemies.contains(enemy.get())) {
                if ((world.clock - hit_enemies[enemy.get()]) < RECOVERY_TIME) {
                    continue;
                }
            }

            if (hammer_collider.collides_with(enemy->body_collider)) {
                hit_enemies[enemy.get()] = world.clock;
                enemy->velocity += raylib::Vector2(-4000, 0).Rotate(rotation);
                world.send_notification(core::ENEMY_FLUNG);
            }
        }
    }
}

void Player::_handle_swing(core::World& world) {
    switch (state) {
        case READY:
            if (IsKeyPressed(KEY_SPACE)) {
                state = SWINGING;
                state_timer = SWING_TIME;
            }

            break;

        case SWINGING:
            rotation += ((3.0 * PI) / SWING_TIME) * GetFrameTime();
            state_timer -= GetFrameTime();

            _handle_hammer_collision(world);

            if (state_timer <= 0) {
                state = RECOVERY;
                state_timer = RECOVERY_TIME;
            }
            break;

        case RECOVERY:
            auto rotation_speed = (2.0 * PI) / RECOVERY_TIME;
            rotation_speed *= Remap(state_timer / RECOVERY_TIME, 1, 0, 1, 0.3);

            rotation += rotation_speed * GetFrameTime();
            state_timer -= GetFrameTime();

            if (state_timer <= 0) {
                state = READY;
            }

            break;
    }
}

void Player::update(core::World& world) {
    _move_player();
    _handle_swing(world);

    health = std::min(PLAYER_HEALTH, health + (GetFrameTime() * 0.01f));
    
    // check screen collision
    auto displacement = player_collider.collide_with_screen();
    if (displacement) {
        velocity = displacement.value().Normalize() * velocity.Length() * 0.8;
        position += displacement.value();
    }

    // check enemy collision
    for (auto e : world.get_entities()) {
        if (auto enemy = std::dynamic_pointer_cast<enemy::Enemy>(e)) {
            if (!enemy->is_on_screen()) continue;

            if (player_collider.collides_with(enemy->body_collider)) {
                health -= GetFrameTime();
                if (health <= 0) {
                    this->destroy();
                    world.send_notification(core::PLAYER_DIED);
                    return;
                }
            }
        }
    }
}

void Player::draw(core::World& world) {
    auto color = raylib::Color(25, 25, 100);

    { // draw hammer handle 
        const float HANDLE_WIDTH = 5.;

        std::vector<raylib::Vector2> points({
            raylib::Vector2(HANDLE_WIDTH, 0.),
            raylib::Vector2(-HANDLE_WIDTH, 0.),
            raylib::Vector2(-HANDLE_WIDTH, hammer_collider.offset.y),
            raylib::Vector2(HANDLE_WIDTH, hammer_collider.offset.y),
        });
        points.insert(std::begin(points), raylib::Vector2());
        points.push_back(points[1]);

        for (auto& point : points) {
            point = point.Rotate(rotation);
            point += position;
        }

        DrawTriangleFan(points.data(), points.size(), BLACK);
    }
    
    { // draw body
        std::vector<raylib::Vector2> points = create_player_shape(PLAYER_SIDES);
        points.insert(std::begin(points), raylib::Vector2());
        points.push_back(points[1]);

        for (auto& point : points) {
            point = point.Rotate(rotation);
            point += position;
        }

        DrawTriangleFan(points.data(), points.size(), color);
    }

    { // draw eyes
        std::vector<raylib::Vector2> eyes({
                raylib::Vector2(-6, -22),
                raylib::Vector2(6, -22),
        });

        for (auto point : eyes) {
            point = point.Rotate(rotation);
            point += position;

            DrawCircleV(point, 4, WHITE);
        }

        auto pupil_radius = Remap(std::max(0.f, health), PLAYER_HEALTH, 0, 1.5, 3);
        for (auto point : eyes) {
            point += raylib::Vector2(0, -1);
            point = point.Rotate(rotation);
            point += position;

            DrawCircleV(point, pupil_radius, BLACK);
        }
    }

    { // draw hammer head
        std::vector<raylib::Vector2> points = create_hammer_head_shape();
        points.insert(std::begin(points), raylib::Vector2());
        points.push_back(points[1]);

        for (auto& point : points) {
            point += hammer_collider.offset;
            point = point.Rotate(rotation);
            point += position;
        }

        DrawTriangleFan(points.data(), points.size(), BLACK);
    }
}
