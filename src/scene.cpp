#include "scene.hpp"
#include "Functions.hpp"
#include "core.hpp"
#include "enemy.hpp"
#include "raylib.h"
#include <cstdlib>
#include <format>
#include <memory>

using namespace game::scene;

FPSLabel::FPSLabel() {
    draw_order = 10;
}

void FPSLabel::draw(core::World& world) {
    if (world.draw_delta == 0 || world.update_delta == 0) return;

    raylib::DrawText(
            std::format("FPS: {0}", std::round(1 / GetFrameTime())),
            -SCREEN_WIDTH / 2 + 10,
            SCREEN_HEIGHT / 2 - 105,
            30,
            Fade(GREEN, 0.5)
    );

    raylib::DrawText(
            std::format("UPDATE_TIME: {0}", world.update_delta),
            -SCREEN_WIDTH / 2 + 10,
            SCREEN_HEIGHT / 2 - 70,
            30,
            Fade(GREEN, 0.5)
    );

    raylib::DrawText(
            std::format("DRAW_TIME: {0}", world.draw_delta),
            -SCREEN_WIDTH / 2 + 10,
            SCREEN_HEIGHT / 2 - 35,
            30,
            Fade(GREEN, 0.5)
    );
}

void spawn_enemy(game::core::World& world) {
        auto enemy = std::make_shared<game::enemy::Enemy>();
        auto angle = randf() * 2 * PI;
        enemy->position = raylib::Vector2(1500, 0).Rotate(angle);

        world.add_entity(enemy);
}

Scene::Scene(core::World& world) {
    // create player
    world.add_entity(std::make_shared<FPSLabel>());
}

void Scene::update(core::World& world) {
    spawn_timer -= GetFrameTime();

    if (spawn_timer <= 0) {
        spawn_timer = SPAWN_TIMER;
        spawn_enemy(world);
    }
}
