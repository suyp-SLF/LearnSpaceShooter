cmake_minimum_required(VERSION 3.10.0)
project(StarLand VERSION 0.1.0 LANGUAGES C CXX)

find_package(SDL2 REQUIRED)

add_executable(StarLand main.cpp)

target_link_libraries(StarLand 
                        SDL2::SDL2  
                        SDL2::SDL2main)