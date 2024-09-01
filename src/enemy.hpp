#include "core.hpp"
#include "physics.hpp"

namespace game::enemy {
    class Enemy : public core::Entity {
        public:
            Enemy();
            void update(core::World& world);
            void draw(core::World& world);
    };
}
