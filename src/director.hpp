#pragma once

#include "core.hpp"

namespace game::director {
    const float STARTING_SPAWN_TIMER = 2.0;
    const float ENDING_SPAWN_TIMER = 0.5;

    class Director : public core::Entity {
        float spawn_timer = 0;
        int score = 0;
        float clock = 0;

        public:
            Director(core::World& world);
            void update();
            void draw() {}

            void player_died();
            void enemy_flung();
            void enemy_killed();

            int get_score();
            float get_clock();
    };
}
