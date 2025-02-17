cmake_minimum_required(VERSION 3.16)
project(SDL3Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем CMake, где искать SDL3
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/sdl3/3.2.0/lib/cmake/SDL3")

find_package(SDL3 REQUIRED)

include(FetchContent)

FetchContent_Declare(
  TBB
  GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
  GIT_TAG v2021.12.0
)

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

# Указываем заголовочные файлы SDL3
target_include_directories(SDL_RayTracer PRIVATE /opt/homebrew/Cellar/sdl3/3.2.0/include)

# Линкуем SDL3 и TBB
target_link_libraries(SDL_RayTracer PRIVATE SDL3::SDL3 TBB::tbb)