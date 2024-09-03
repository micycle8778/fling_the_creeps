#include "particles.hpp"
#include "Vector2.hpp"
#include "raylib.h"

using namespace game::particles;

Particle::Particle(core::World& world, float lifetime) : core::Entity(world) {
    this->starting_lifetime = lifetime;
    this->lifetime = lifetime;

    draw_order = -10;
}

void Particle::update() {
    lifetime -= GetFrameTime();
    if (lifetime <= 0) {
        this->destroy();
        return;
    }

    position += linear_velocity * GetFrameTime();
    rotation += angular_velocity * GetFrameTime();

    linear_velocity -= linear_velocity * linear_drag * GetFrameTime();
    angular_velocity -= angular_velocity * angular_drag * GetFrameTime();
}

void Particle::draw() {
    auto draw_color = color;
    draw_color.a *= lifetime / starting_lifetime;

    switch(shape) {
        case CIRCLE:
            // trivial, just draw the circle
            DrawCircleV(position, radius, draw_color);
            break;
        case SQUARE:
            // transform square points, then draw
            std::vector<raylib::Vector2> points({
                raylib::Vector2(),
                raylib::Vector2(radius / 2, -radius / 2),
                raylib::Vector2(-radius / 2, -radius / 2),
                raylib::Vector2(-radius / 2, radius / 2),
                raylib::Vector2(radius / 2, radius / 2),
                raylib::Vector2(radius / 2, -radius / 2)
            });

            for (auto& p : points) {
                p = p.Rotate(rotation);
                p += position;
            }

            DrawTriangleFan(points.data(), points.size(), draw_color);

    }
}

