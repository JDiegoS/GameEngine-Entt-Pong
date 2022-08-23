cmake_minimum_required(VERSION 3.16)

project(MYGAME VERSION 1.0)

add_executable(${PROJECT_NAME} src/main.cpp src/Game/game.cpp src/Game/game.h)


target_link_libraries(
    ${PROJECT_NAME}
    SDL2
    SDL2_image
    SDL2main
)

