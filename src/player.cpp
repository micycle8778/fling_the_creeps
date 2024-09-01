#include <iostream>
#include <algorithm>

#include "Color.hpp"
#include "Vector2.hpp"
#include "player.hpp"
#include "physics.hpp"

#include "raylib.h"

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

        // point.x = std::min(point.x, 0.6f);

        result.push_back(point * 35.);
    }

    return result;
}

std::vector<raylib::Vector2> create_hammer_head_shape() {
    const raylib::Vector2 HAMMER_HEAD_HALF_SIZE(50, 30);

    return std::vector<raylib::Vector2>({
        HAMMER_HEAD_HALF_SIZE * raylib::Vector2(1, 1),
        HAMMER_HEAD_HALF_SIZE * raylib::Vector2(1, -1),
        HAMMER_HEAD_HALF_SIZE * raylib::Vector2(-1, -1),
        HAMMER_HEAD_HALF_SIZE * raylib::Vector2(-1, 1),
    });
}

Player::Player() : 
    player_collider(this, create_player_shape(PLAYER_SIDES)),
    hammer_collider(this, create_hammer_head_shape())
{
    hammer_collider.offset = raylib::Vector2(0, 125);
}

void Player::update(core::World& _world) {
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

    if (input != raylib::Vector2()) { // TODO: use exp decay instead
        float desired_angle = (input * raylib::Vector2(1, -1)).Angle(raylib::Vector2(1., 0.));
        rotation = lerp_angle(rotation, desired_angle, 0.025);
    }

    // check collision
    auto displacement = player_collider.collide_with_screen();
    if (displacement != raylib::Vector2()) {
        velocity = displacement.Normalize() * velocity.Length() * 0.8;
        position += displacement;
    }
}

void Player::draw(core::World& world) {
    auto& enemy = world.get_entities()[1];

    auto color = raylib::Color(25, 25, 100);
    if (player_collider.collides_with(*enemy->colliders[0])) {
        color = RED;
    }

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
