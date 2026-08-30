cmake_minimum_required(VERSION 3.14)
project(Tetris LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_executable(RatTetris
    main.cpp
    models/Tetromino.cpp
    models/Board.cpp
    models/Game.cpp
    handlers/PieceGenerator.cpp
    handlers/LevelManager.cpp
    handlers/Renderer.cpp
)
target_include_directories(RatTetris PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/handlers
)
target_link_libraries(RatTetris PRIVATE raylib)
if (APPLE)
    target_link_libraries(RatTetris PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()