#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <SDL3/SDL_render.h>

#include "math/vec2.h"

namespace boids {
    struct Boid {
        vec2 pos;
        vec2 velocity;
        // float angle; // in rad
        // float steer; // in rad/s
        SDL_Color color;
    };

    class Boids {
    public:
        std::vector<Boid> boids;
        vec2 mapSize;
        float minSpeed;
        float maxSpeed;
        std::default_random_engine generator = std::default_random_engine();
        std::uniform_real_distribution<float> distribution = std::uniform_real_distribution<float>(0, 1);

        float separationRadius = 15;
        float separationWeight = 0.1;
        float alignmentRadius = 150;
        float alignmentWeight = 0.01;
        float cohesionRadius = 500;
        float cohesionWeight = 0.0005;

        // int biasedBoids;
        float biasRadius = 300;
        float biasWeight = 0.05;

        Boids(int size, vec2 mapSize, float minSpeed, float maxSpeed) {
            // biasedBoids = size/2;
            this->mapSize = mapSize;
            this->minSpeed = minSpeed;
            this->maxSpeed = maxSpeed;
            for (int i = 0; i < size; i++) {
                vec2 pos = vec2{distribution(generator) * mapSize.x, distribution(generator) * mapSize.y};
                float angle = distribution(generator) * std::numbers::pi;
                vec2 velocity = vec2{std::cos(angle), std::sin(angle)} * (minSpeed + maxSpeed) / 2;
                SDL_Color color = {
                    static_cast<Uint8>(distribution(generator) * 255),
                    static_cast<Uint8>(distribution(generator) * 255),
                    static_cast<Uint8>(distribution(generator) * 255), 255
                };
                boids.push_back({pos, velocity, color});
            }
        }

        void update(float dt, vec2 biasPos) {
            // update steer
            for (int i=0;i<boids.size();i++) {
                Boid &boid = boids[i];
                vec2 separation = vec2();
                vec2 alignmentVelocity = vec2();
                int alignmentCount = 0;
                vec2 cohesionPos = vec2();
                int cohesionCount = 0;
                for (auto &other: boids) {
                    if (&boid == &other) continue;
                    vec2 diff = other.pos - boid.pos;
                    float distance = diff.magnitude();
                    if (distance < separationRadius) {
                        separation += diff;
                    }
                    if (distance < alignmentRadius) {
                        alignmentVelocity += other.velocity;
                        alignmentCount++;
                    }
                    if (distance < cohesionRadius) {
                        cohesionPos += other.pos;
                        cohesionCount++;
                    }
                }

                boid.velocity -= separation * separationWeight;

                if (alignmentCount > 0) {
                    alignmentVelocity /= (float)alignmentCount;
                    boid.velocity += (alignmentVelocity - boid.velocity) * alignmentWeight;
                }

                if (cohesionCount > 0) {
                    cohesionPos /= (float)cohesionCount;
                    boid.velocity += (cohesionPos - boid.pos) * cohesionWeight;
                }

                float biasDistance = (boid.pos - biasPos).magnitude();
                if (biasDistance < biasRadius) {
                    vec2 biasDir = biasPos - boid.pos;
                    boid.velocity += biasDir * biasWeight * biasDistance / biasRadius;
                }

                float speed = boid.velocity.magnitude();
                if (speed < minSpeed) {
                    boid.velocity = boid.velocity * (minSpeed / speed);
                }
                if (speed > maxSpeed) {
                    boid.velocity = boid.velocity * (maxSpeed / speed);
                }
            }

            // std::cout << boids[0].steer << std::endl;

            // update angle and position
            for (auto &boid: boids) {
                // boid.angle += boid.steer * dt;
                // if (boid.angle > 2 * std::numbers::pi) boid.angle -= 2 * std::numbers::pi;
                // if (boid.angle < 0) boid.angle += 2 * std::numbers::pi;

                boid.pos += boid.velocity * dt;
                if (boid.pos.x < 0) boid.pos.x += mapSize.x;
                if (boid.pos.x > mapSize.x) boid.pos.x -= mapSize.x;
                if (boid.pos.y < 0) boid.pos.y += mapSize.y;
                if (boid.pos.y > mapSize.y) boid.pos.y -= mapSize.y;
            }
        }

        void render(SDL_Renderer *);
    };
} // boids
