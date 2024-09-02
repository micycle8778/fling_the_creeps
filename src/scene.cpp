#include "scene.hpp"
#include "enemy.hpp"
#include "raylib.h"
#include <cstdlib>

using namespace game::scene;

Scene::Scene() {
    // create player
}

void Scene::update(core::World& world) {
    spawn_timer -= GetFrameTime();

    if (spawn_timer <= 0) {
        spawn_timer = SPAWN_TIMER;

        auto enemy = std::make_shared<enemy::Enemy>();
        auto angle = randf() * 2 * PI;
        enemy->position = raylib::Vector2(1500, 0).Rotate(angle);

        world.add_entity(enemy);
    }
}
