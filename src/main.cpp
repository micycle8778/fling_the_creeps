#include <memory>

#include "raylib-cpp.hpp"

#include "core.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "raylib.h"

int main() {
    raylib::Window window(
            1280, 720, "fling_the_creeps",
            FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE
    );

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    game::core::World world;

    world.add_entity(std::make_unique<game::player::Player>());
    world.add_entity(std::make_unique<game::enemy::Enemy>());

    while (!window.ShouldClose()) {
        world.update();
    }

    return 0;
}

