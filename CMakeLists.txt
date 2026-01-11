#标题
cmake_minimum_required(VERSION 3.10.0)
project(StarLand VERSION 0.1.0 LANGUAGES C CXX)

#设置C++标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#设置编译选项
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)#-Werror
endif()

#设置编译目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR})

set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

#设置头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

#查找并载入CMake预设
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

#添加可执行文件
add_executable(${TARGET} 
                src/main.cpp
                src/Game.cpp
                src/SceneTitle.cpp
                src/SceneMain.cpp
                src/SceneEnd.cpp
                src/Scene.cpp
                )

#链接库
target_link_libraries(${TARGET}
                        ${SDL2_LIBRARIES}
                        SDL2_image::SDL2_image
                        SDL2_mixer::SDL2_mixer
                        SDL2_ttf::SDL2_ttf)