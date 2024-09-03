#pragma once 

#include "core.hpp"

namespace game::gui {
    class FPSLabel : public core::Entity {
        bool visible = false;

        public:
            FPSLabel(core::World& world);
            void update();
            void draw();
    };

    class ScoreLabel : public core::Entity {
        public:
            bool visible = true;

            ScoreLabel(core::World& world);
            void update();
            void draw();
    };
}
