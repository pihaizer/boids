cmake_minimum_required(VERSION 3.28.2)
project(Boids)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 CONFIG REQUIRED)
#find_package(imgui CONFIG REQUIRED)

# imgui locally

set(IMGUI_PATH  "src/imgui-1.91.4")
file(GLOB IMGUI_SOURCES
        ${IMGUI_PATH}/*.cpp
        ${IMGUI_PATH}/backends/imgui_impl_sdl3.cpp
        ${IMGUI_PATH}/backends/imgui_impl_sdlrenderer3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_PATH})

target_link_libraries(imgui PRIVATE SDL3::SDL3)

add_executable(Boids src/main.cpp
        src/application.cpp
        src/application.h
        src/boids.cpp
        src/boids.h
        src/math/vec2.h
)

target_include_directories(Boids PUBLIC ${IMGUI_PATH})

# SDL3::SDL3main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL3::SDL3main)
    # It has an implicit dependency on SDL3 functions, so it MUST be added before SDL3::SDL3 (or SDL3::SDL3-static)
    target_link_libraries(Boids PRIVATE SDL3::SDL3main)
endif()

target_link_libraries(Boids PRIVATE
        SDL3::SDL3
        imgui
)