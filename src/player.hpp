#include "core.hpp"
#include "physics.hpp"

namespace game::player {
    class Player : public game::core::Entity {
        raylib::Vector2 velocity;
        physics::Collider player_collider;
        
        public:
            Player();
            void update();
            void draw();
    };
}

