cmake_minimum_required(VERSION 3.16)
project(SDL3Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.0
)

FetchContent_Declare(
  TBB
  GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
  GIT_TAG v2021.12.0
)

FetchContent_MakeAvailable(SDL3)
FetchContent_MakeAvailable(TBB)

add_executable(SDL_RayTracer 
    Source/Constants.cpp
    Source/ControllAppManager.cpp
    Source/Helper.cpp
    Source/Main.cpp
    Source/ProfileMertickCollector.cpp
    Source/RayTrace.cpp
    Source/RenderManager.cpp
    Source/ThreadPoolManager.cpp
)

target_link_libraries(SDL_RayTracer PRIVATE SDL3::SDL3 TBB::tbb)