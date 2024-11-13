#pragma once

#include <memory>
#include <span>
#include <vector>
#include <map>

#include "Vector2.hpp"

float randf(); 

namespace game::director {
    class Director;
}

namespace game::player {
    class Player;
}

namespace game::physics {
    struct Collider;
}

class Foo {
    public:
        Foo(int x);
};

namespace game {
    const float SCREEN_WIDTH = 1280;
    const float SCREEN_HEIGHT = 720;

    namespace core {
        enum ProcessMode {
            NEVER = 0,
            WHEN_UNPAUSED = 0b01,
            WHEN_PAUSED = 0b10,
            ALWAYS = 0b11,
        };

        class World;

        class Entity {
            bool destroyed = false;

            protected:
                core::World& world;
                ProcessMode process_mode = WHEN_UNPAUSED;
                ProcessMode draw_mode = ALWAYS;

            public:
                raylib::Vector2 position = raylib::Vector2();
                float rotation = 0;
                bool processing = true;

                std::vector<physics::Collider*> colliders;

                int draw_order = 0;

                Entity(core::World& world) : world(world) {}

                virtual void update() = 0;
                virtual void draw() = 0;

                bool is_destroyed();
                void destroy();

                ProcessMode get_process_mode();
        };

        class World {
            std::vector<std::shared_ptr<Entity>> entities;
            std::map<int, std::vector<std::shared_ptr<Entity>>> draw_map;
            std::vector<Entity*> to_be_freed;

            bool vsync = true;

            bool paused = false;

            public:
                float clock = 0;

                float update_delta = 0;
                float draw_delta = 0;
                
                std::shared_ptr<player::Player> player;
                std::shared_ptr<director::Director> director;

                void add_entity(std::shared_ptr<Entity> entity);
                void update();
                std::span<std::shared_ptr<Entity>> get_entities();
        };
    }
}
