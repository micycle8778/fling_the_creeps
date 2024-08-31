#include <iostream>
#include <algorithm>

#include "Vector2.hpp"
#include "player.hpp"
#include "physics.hpp"

#include "raylib.h"

using namespace game::player;

const float ACCELERATION = 6000.;
const float MAX_SPEED = 1500.;

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

        point.x = std::min(point.x, 0.6f);

        result.push_back(point * 50.);
    }

    return result;
}

Player::Player() : player_collider(this, create_player_shape(10)) {
    colliders.push_back(player_collider);
}

void Player::update() {
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

    if (input != raylib::Vector2()) {
        float desired_angle = (input * raylib::Vector2(1, -1)).Angle(raylib::Vector2(1., 0.));
        rotation = lerp_angle(rotation, desired_angle, 0.1);
    }

    std::cout << "rotation = " << rotation << std::endl;

    // check collision
    auto displacement = player_collider.collide_with_screen();
    if (displacement != raylib::Vector2()) {
        velocity = displacement.Normalize() * velocity.Length() * 0.8;
        position += displacement;
    }
}

void Player::draw() {
    std::vector<raylib::Vector2> points = create_player_shape(50);
    
    points.insert(std::begin(points), raylib::Vector2());
    points.push_back(points[1]);

    for (auto& point : points) {
        point = point.Rotate(rotation);
        point += position;
    }

    DrawTriangleFan(points.data(), points.size(), BLACK);
}
