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
#include "director.hpp"

// #define DRAW_COLLIDERS

#ifdef DRAW_COLLIDERS
#include <iterator>
#include "physics.hpp"
#endif

using namespace game;
using namespace core;

const std::chrono::microseconds ONE_SECOND{1};

bool destroyed_this_frame = false;

float randf() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

bool Entity::is_destroyed() {
    return destroyed;
}

void Entity::destroy() {
    destroyed = true;
    destroyed_this_frame = true;
}

void World::add_entity(std::shared_ptr<Entity> entity) {
    if (auto player = std::dynamic_pointer_cast<player::Player>(entity)) {
        this->player = player;
    }

    if (auto director = std::dynamic_pointer_cast<director::Director>(entity)) {
        this->director = director;
    }

    
    entities.push_back(entity);

    if (!draw_map.contains(entity->draw_order)) {
        draw_map[entity->draw_order] = std::vector<std::shared_ptr<Entity>>();
    }     

    draw_map[entity->draw_order].push_back(entity);
}

void World::update() {
    if (vsync)
        SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    else 
        SetTargetFPS(0);

    if (IsKeyPressed(KEY_F9)) 
        vsync = !vsync;

    clock += GetFrameTime();

    {
        auto now = std::chrono::steady_clock::now();

        destroyed_this_frame = false;

        for (int idx = 0; idx < entities.size(); idx++) {
            auto e = entities[idx];
            if (e->is_destroyed()) continue;

            e->update();
        }

        if (destroyed_this_frame) {
            for (int idx = entities.size() - 1; idx >= 0; idx--) {
                auto e = entities[idx];
                if (e->is_destroyed()) {
                    auto last_entity = entities.back();
                    entities.pop_back();

                    if (idx != entities.size())
                        entities[idx] = last_entity;
                }
            }

            for (auto& [key, val] : draw_map) {
                for (int idx = val.size() - 1; idx >= 0; idx--) {
                    auto e = val[idx];
                    if (e->is_destroyed()) {
                        auto last_entity = val.back();
                        val.pop_back();

                        if (idx != val.size())
                            val[idx] = last_entity;
                    }
                }
            }
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

                for (auto const& [key, val] : draw_map) {
                    for (auto const& e : val) {
                        e->draw();
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
