cmake_minimum_required(VERSION 3.10.0)
project(StarLand VERSION 0.1.0 LANGUAGES C CXX)

include_directories("/opt/homebrew/opt/sdl2/include")
link_directories("/opt/homebrew/opt/sdl2/lib")

add_executable(StarLand main.cpp)

target_link_libraries(StarLand SDL2 SDL2main)