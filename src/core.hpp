#pragma once

#include <memory>
#include <span>
#include <vector>

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
            public:
                raylib::Vector2 position = raylib::Vector2();
                float rotation = 0;
                bool processing = true;

                std::vector<physics::Collider*> colliders;

                virtual void update(World& world) = 0;
                virtual void draw(World& world) = 0;
                virtual void recieve_notification(World& _world, Notification _notification) {}
        };

        class World {
            std::vector<std::shared_ptr<Entity>> entities;
            std::vector<Entity*> to_be_freed;

            public:
                float clock = 0;
                std::shared_ptr<player::Player> player;

                void add_entity(std::shared_ptr<Entity> entity);
                void update();
                std::span<std::shared_ptr<Entity>> get_entities();
                void destroy(Entity* entity);
                void send_notification(Notification notification);
        };
    }
}
