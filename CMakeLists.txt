cmake_minimum_required(VERSION 2.8)

project(zelluf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -D_REENTRANT")

include_directories(/usr/include/SDL2)

add_executable(zelluf ${PROJECT_SOURCE_DIR}/zelluf.c)
TARGET_LINK_LIBRARIES(zelluf -lsimple2d -L/usr/lib/x86_64-linux-gnu -lSDL2 -lGL -lm -lSDL2_image -lSDL2_mixer -lSDL2_ttf)

add_executable(bouncer ${PROJECT_SOURCE_DIR}/bouncer.c)
TARGET_LINK_LIBRARIES(bouncer -lsimple2d -L/usr/lib/x86_64-linux-gnu -lSDL2 -lGL -lm -lSDL2_image -lSDL2_mixer -lSDL2_ttf)

