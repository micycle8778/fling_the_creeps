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

// #define DRAW_COLLIDERS

using namespace game;
using namespace core;

void World::add_entity(std::shared_ptr<Entity> entity) {
    entities.push_back(entity);
}

void World::update() {
    for (auto e : entities) {
        e->update();
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

            for (auto e : entities) {
                e->draw();

#ifdef DRAW_COLLIDERS
                for (auto c : e->colliders) {
                    auto points = c.points;
                    points.insert(std::begin(points), raylib::Vector2());
                    points.push_back(points[1]);

                    for (auto& p : points) {
                        p = p.Rotate(e->rotation);
                        p += e->position;
                        p += c.offset;
                    }

                    DrawTriangleFan(points.data(), points.size(), c.debug_color);
                }
#endif
            }

        camera.EndMode();
    EndDrawing();
}
