#include "Vector2.hpp"
#include "core.hpp"

namespace game::particles {
    enum Shape {
        CIRCLE, SQUARE
    };

    class Particle : public core::Entity {
        float starting_lifetime = 0;
        float lifetime = 0;

        public:

            raylib::Vector2 linear_velocity;
            float angular_velocity;

            float linear_drag = 0;
            float angular_drag = 0;

            Color color = BLACK;
            float radius = 10;

            Shape shape = CIRCLE;

            Particle(float lifetime);
            void update(core::World& world);
            void draw(core::World& world);
    };
}
