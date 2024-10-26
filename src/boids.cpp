#include <SDL3/SDL_oldnames.h>

#include "Boids.h"

namespace boids {
    void Boids::render(SDL_Renderer* renderer) {
        for (auto &boid : boids) {
            SDL_SetRenderDrawColor(renderer, boid.color.r, boid.color.g, boid.color.b, boid.color.a);
            // SDL_RenderLine(renderer, boid.pos.x, boid.pos.y, boid.pos.x + 10 * std::cos(boid.angle),
                               // boid.pos.y + 10 * std::sin(boid.angle));
            vec2 direction = boid.velocity.normalized();
            vec2 start = boid.pos - direction.scaled(10);
            SDL_RenderLine(renderer, start.x, start.y, boid.pos.x, boid.pos.y);
        }
    }
} // boids