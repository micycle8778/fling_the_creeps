#include "gui.hpp"
#include "director.hpp"
#include "Functions.hpp"
#include <format>

using namespace game::gui;

FPSLabel::FPSLabel(core::World& world) : core::Entity(world) {
    draw_order = 10;
}

void FPSLabel::update() {
    if (IsKeyPressed(KEY_F3))
        visible = !visible;
}

void FPSLabel::draw() {
    if (!visible) return;
    if (world.draw_delta == 0 || world.update_delta == 0) return;

    raylib::DrawText(
            std::format("FPS: {0}", std::round(1 / GetFrameTime())),
            -SCREEN_WIDTH / 2 + 10,
            SCREEN_HEIGHT / 2 - 105,
            30,
            Fade(GREEN, 0.5)
    );

    raylib::DrawText(
            std::format("UPDATE_TIME: {0}", world.update_delta),
            -SCREEN_WIDTH / 2 + 10,
            SCREEN_HEIGHT / 2 - 70,
            30,
            Fade(GREEN, 0.5)
    );

    raylib::DrawText(
            std::format("DRAW_TIME: {0}", world.draw_delta),
            -SCREEN_WIDTH / 2 + 10,
            SCREEN_HEIGHT / 2 - 35,
            30,
            Fade(GREEN, 0.5)
    );
}

ScoreLabel::ScoreLabel(core::World& world) : core::Entity(world) {
    draw_order = 10;
}

void ScoreLabel::update() {}

void ScoreLabel::draw() {
    if (!visible) return;

    raylib::DrawText(
            std::format("SCORE: {0}", world.director->get_score()).c_str(),
            -SCREEN_WIDTH / 2 + 10,
            -SCREEN_HEIGHT / 2 + 10,
            45,
            RED
    );
}
