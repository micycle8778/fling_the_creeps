#pragma once

#include <memory>
#include <span>
#include <vector>
#include <map>

#include "Vector2.hpp"

float randf(); 

namespace game::player {
    class Player;
}

namespace game::physics {
    struct Collider;
}

namespace game {
    const float SCREEN_WIDTH = 1280;
    const float SCREEN_HEIGHT = 720;

    namespace core {
        enum Notification {
            ENEMY_FLUNG,
            PLAYER_DIED,

        };

        class World;

        class Entity {
            bool destroyed = false;

            public:
                raylib::Vector2 position = raylib::Vector2();
                float rotation = 0;
                bool processing = true;

                std::vector<physics::Collider*> colliders;

                int draw_order = 0;

                virtual void update(World& world) = 0;
                virtual void draw(World& world) = 0;
                virtual void recieve_notification(World& _world, Notification _notification) {}

                bool is_destroyed();
                void destroy();
        };

        class World {
            std::vector<std::shared_ptr<Entity>> entities;
            std::map<int, std::vector<std::shared_ptr<Entity>>> draw_map;
            std::vector<Entity*> to_be_freed;

            bool vsync = true;

            public:
                float clock = 0;

                float update_delta = 0;
                float draw_delta = 0;
                
                std::shared_ptr<player::Player> player;

                void add_entity(std::shared_ptr<Entity> entity);
                void update();
                std::span<std::shared_ptr<Entity>> get_entities();
                void send_notification(Notification notification);
        };
    }
}
