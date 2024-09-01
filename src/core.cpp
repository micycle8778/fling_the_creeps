#include "Camera2D.hpp"
#include "Vector2.hpp"
#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <memory>
#include "core.hpp"
#include "player.hpp"

// #define DRAW_COLLIDERS

#ifdef DRAW_COLLIDERS
#include <iterator>
#include "physics.hpp"
#endif

using namespace game;
using namespace core;

void World::add_entity(std::shared_ptr<Entity> entity) {
    if (auto ptr = std::dynamic_pointer_cast<player::Player>(entity)) {
        player = ptr;
    }
    entities.push_back(std::move(entity));
}

void World::update() {
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    clock += GetFrameTime();

    for (auto& e : entities) {
        e->update(*this);
    }

    auto camera = raylib::Camera2D();
    camera.target = raylib::Vector2();
    camera.offset = raylib::Vector2(GetScreenWidth() / 2., GetScreenHeight() / 2.);
    camera.zoom = std::min(GetScreenWidth() / SCREEN_WIDTH, GetScreenHeight() / SCREEN_HEIGHT);
    camera.rotation = 0.0;

    BeginDrawing();
        ClearBackground(BLACK);

        camera.BeginMode();

            // background
            DrawRectangle(
                -SCREEN_WIDTH / 2.,
                -SCREEN_HEIGHT / 2.,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                WHITE
            );

            for (auto& e : entities) {
                e->draw(*this);

#ifdef DRAW_COLLIDERS
                for (auto c : e->colliders) {
                    auto points = c->get_transformed_points();

                    points.insert(std::begin(points), e->position + c->offset.Rotate(e->rotation));
                    points.push_back(points[1]);

                    DrawTriangleFan(points.data(), points.size(), c->debug_color);
                }
#endif
            }

        camera.EndMode();
    EndDrawing();
}

std::span<std::shared_ptr<Entity>> World::get_entities() {
    return std::span<std::shared_ptr<Entity>>(entities);
}
