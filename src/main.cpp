#include <memory>

#include "raylib-cpp.hpp"

#include "core.hpp"
#include "player.hpp"
#include "raylib.h"

int main() {
    raylib::Window window(
            1280, 720, "fling_the_creeps",
            FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE
    );

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    game::core::World world;

    auto ptr = std::static_pointer_cast<game::core::Entity>(std::make_shared<game::player::Player>());
    world.add_entity(ptr);

    while (!window.ShouldClose()) {
        world.update();
    }

    return 0;
}

