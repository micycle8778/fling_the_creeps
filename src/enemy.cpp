#include "enemy.hpp"
#include "Vector2.hpp"

using namespace game::enemy;

std::vector<raylib::Vector2> get_enemy_shape() {
    return std::vector<raylib::Vector2>({
        raylib::Vector2(80, 0),
        raylib::Vector2(0, -35),
        raylib::Vector2(-80, 0),
        raylib::Vector2(0, 35)
    });
}

Enemy::Enemy() {
    colliders.push_back(physics::Collider(this, get_enemy_shape()));
}

void Enemy::update(core::World& _world) {}

void Enemy::draw(core::World& _world) {
    auto points = get_enemy_shape();
    points.insert(std::begin(points), raylib::Vector2());
    points.push_back(points[1]);

    DrawTriangleFan(points.data(), points.size(), BLACK);
}
