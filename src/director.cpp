#include "director.hpp"
#include "core.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include "raylib.h"
#include "gui.hpp"
#include <cstdlib>
#include <memory>

using namespace game::director;

void spawn_enemy(game::core::World& world) {
        auto enemy = std::make_shared<game::enemy::Enemy>(world);
        auto angle = randf() * 2 * PI;
        enemy->position = raylib::Vector2(1500, 0).Rotate(angle);

        world.add_entity(enemy);
}

Director::Director(core::World& world) : core::Entity(world) {
    world.add_entity(std::make_shared<player::Player>(world));
    world.add_entity(std::make_shared<gui::FPSLabel>(world));
    world.add_entity(std::make_shared<gui::ScoreLabel>(world));

    // for (int idx = 0; idx < 200; idx++)
    //     spawn_enemy(world);
}

void Director::update() {
    if (!world.player->is_destroyed())
        clock += GetFrameTime();

    spawn_timer -= GetFrameTime();

    if (spawn_timer <= 0) {
        spawn_timer = Remap(std::min(clock, 1001.f), 0, 240, STARTING_SPAWN_TIMER, ENDING_SPAWN_TIMER);
        spawn_enemy(world);
    }
}

void Director::enemy_flung() {
    score += 1;
}
void Director::enemy_killed() {
    score += 3;
}

void Director::player_died() {}

int Director::get_score() {
    return score;
}

float Director::get_clock() {
    return clock;
}
