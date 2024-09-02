#include "Camera2D.hpp"
#include "Vector2.hpp"
#include "raylib.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <vector>
#include <memory>
#include "core.hpp"
#include "player.hpp"
#include "physics.hpp"

// #define DRAW_COLLIDERS

#ifdef DRAW_COLLIDERS
#include <iterator>
#endif

using namespace game;
using namespace core;

const std::chrono::microseconds ONE_SECOND{1};

float randf() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

void World::add_entity(std::shared_ptr<Entity> entity) {
    if (auto ptr = std::dynamic_pointer_cast<player::Player>(entity)) {
        player = ptr;
    }
    entities.push_back(std::move(entity));
}

void World::update() {
    if (vsync)
        SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    else 
        SetTargetFPS(0);

    if (IsKeyPressed(KEY_F3)) 
        vsync = !vsync;

    clock += GetFrameTime();

    {
        auto now = std::chrono::steady_clock::now();

        for (int idx = 0; idx < entities.size(); idx++) {
            auto e = entities[idx];
            e->update(*this);
        }

        update_delta = (std::chrono::steady_clock::now() - now) / ONE_SECOND;
        update_delta /= 1000;
    }

    auto camera = raylib::Camera2D();
    camera.target = raylib::Vector2();
    camera.offset = raylib::Vector2(GetScreenWidth() / 2., GetScreenHeight() / 2.);
    camera.zoom = std::min(GetScreenWidth() / SCREEN_WIDTH, GetScreenHeight() / SCREEN_HEIGHT);
    camera.rotation = 0.0;

    {
        auto now = std::chrono::steady_clock::now();

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
                        DrawCircleV(e->position, c->radius, c->debug_color);
                    }
#endif
                }

            camera.EndMode();
        EndDrawing();

        draw_delta = (std::chrono::steady_clock::now() - now) / ONE_SECOND;
        draw_delta /= 1000;
    }
}

std::span<std::shared_ptr<Entity>> World::get_entities() {
    return std::span<std::shared_ptr<Entity>>(entities);
}

void World::destroy(Entity* entity) {
    // WARN: this could drop a frame
    if (entities.back().get() == entity) {
        entities.pop_back();
    }

    for (auto& e : entities) {
        if (e.get() == entity) {
            e = entities.back();
            entities.pop_back();
            return;
        }
    }
}

void World::send_notification(Notification notification) {
    for (auto e : entities) {
        e->recieve_notification(*this, notification);
    }
}
