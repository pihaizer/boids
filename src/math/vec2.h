#pragma once

// #include "SDL3/SDL.h"
#include <cmath>
#include <numbers>

namespace boids {
    struct vec2 {
        float x;
        float y;

        vec2(float x, float y) {
            this->x = x;
            this->y = y;
        }

        // vec2(c2v c2v) {
        //     this->x = c2v.x;
        //     this->y = c2v.y;
        // }

        vec2() {
            this->x = 0;
            this->y = 0;
        }

        // operator c2v() const { return c2v{x, y}; }

        vec2 operator+(vec2 other) {
            return vec2(x + other.x, y + other.y);
        }

        vec2 &operator+=(const vec2 &rhs) {
            this->x += rhs.x;
            this->y += rhs.y;
            return *this;
        }

        vec2 operator-() const {
            return vec2(-x, -y);
        }

        vec2 operator-(vec2 other) {
            return vec2(x - other.x, y - other.y);
        }

        vec2 &operator-=(const vec2 &rhs) {
            this->x -= rhs.x;
            this->y -= rhs.y;
            return *this;
        }

        vec2 operator*(float scalar) {
            return vec2(x * scalar, y * scalar);
        }

        vec2 &operator*=(float scalar) {
            this->x *= scalar;
            this->y *= scalar;
            return *this;
        }

        vec2 operator/(float scalar) {
            return vec2(x / scalar, y / scalar);
        }

        vec2 &operator/=(float scalar) {
            this->x /= scalar;
            this->y /= scalar;
            return *this;
        }

        vec2 scaled(float by) {
            return vec2(x * by, y * by);
        }

        vec2 scaled(vec2 by) {
            return vec2(x * by.x, y * by.y);
        }

        vec2 normalized() {
            if (x == 0 && y == 0) {
                return vec2();
            }

            float length = magnitude();
            return vec2(x / length, y / length);
        }

        float magnitude() {
            return sqrt(sqrMagnitude());
        }

        float sqrMagnitude() {
            return x * x + y * y;
        }

        vec2 lerp(vec2 other, float t) {
            if (t < 0)
                t = 0;
            if (t > 1)
                t = 1;
            return *this * (1 - t) + other * t;
        }

        vec2 lerp_unclamped(vec2 other, float t) {
            return *this * (1 - t) + other * t;
        }

        // angle in degrees
        vec2 rotated(float angle, vec2 around = vec2(0, 0)) {
            vec2 p = *this;
            if (angle == 0)
                return p;

            float angle_in_rads = angle / 180.0f * std::numbers::pi;

            float s = sin(angle_in_rads);
            float c = cos(angle_in_rads);

            // translate point back to origin:
            p = p - around;

            // rotate point
            vec2 newP = vec2(
                p.x * c - p.y * s,
                p.x * s + p.y * c);

            // translate point back:
            newP = newP + around;

            return newP;
        }

        static vec2 from_angle(float angle) {
            return {cos(angle), sin(angle)};
        }

        float angle() {
            return atan2(y, x);
        }
    };
} // boids
