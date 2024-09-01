#include "core.hpp"
#include "physics.hpp"
#include "enemy.hpp"
#include <memory>
#include <unordered_map>

namespace game::player {
    enum State {
        READY, SWINGING, RECOVERY
    };

    class Player : public game::core::Entity {
        std::unordered_map<enemy::Enemy*, float> hit_enemies;

        raylib::Vector2 velocity;
        physics::Collider player_collider;
        physics::Collider hammer_collider;

        State state = State::READY;
        float state_timer = 0;
        
        void _move_player();
        void _handle_swing(core::World& world);
        void _handle_hammer_collision(core::World& world);
        
        public:
            Player(core::World& world);
            void update(core::World& world);
            void draw(core::World& world);
    };
}

