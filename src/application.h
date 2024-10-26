#pragma once
#include <cstdint>

// #include "SDL"

namespace boids {
    class Application {
    public:
        int Run();

    private:
        uint64_t m_lastFrameTicks = 0;
        uint64_t m_deltaTicks = 0;
        float m_deltaTime = 0;
    };
}
