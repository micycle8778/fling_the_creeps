#include <memory>

#include "raylib-cpp.hpp"

#include "core.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "raylib.h"
#include "scene.hpp"

int main() {
    raylib::Window window(
            1280, 720, "fling_the_creeps",
            FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE
    );
    SetExitKey(KEY_NULL);


    game::core::World world;

    world.add_entity(std::make_unique<game::player::Player>(world));
    world.add_entity(std::make_unique<game::scene::Scene>(world));

    while (!window.ShouldClose()) {
        world.update();
    }

    return 0;
}

