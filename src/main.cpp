#include <memory>

#include "raylib-cpp.hpp"

#include "core.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "raylib.h"
#include "director.hpp"

int main() {
    raylib::Window window(
            1280, 720, "fling_the_creeps",
            FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE
    );
    SetExitKey(KEY_NULL);


    game::core::World world;

    world.add_entity(std::make_unique<game::director::Director>(world));

    while (!window.ShouldClose()) {
        world.update();
    }

    return 0;
}

