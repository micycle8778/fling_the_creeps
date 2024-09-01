#include "Camera2D.hpp"
#include "Vector2.hpp"
#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>
#include <memory>
#include "core.hpp"
#include "physics.hpp"

#define DRAW_COLLIDERS

using namespace game;
using namespace core;

void World::add_entity(std::unique_ptr<Entity> entity) {
    entities.push_back(std::move(entity));
}

void World::update() {
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
                    auto points = c.get_transformed_points();

                    points.insert(std::begin(points), e->position + c.offset.Rotate(e->rotation));
                    points.push_back(points[1]);

                    DrawTriangleFan(points.data(), points.size(), c.debug_color);
                }
#endif
            }

        camera.EndMode();
    EndDrawing();
}

std::vector<std::unique_ptr<Entity>>& World::get_entities() {
    return entities;
}
