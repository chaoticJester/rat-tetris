cmake_minimum_required(VERSION 3.14)   # 3.14+ รองรับ FetchContent

project(Tetris LANGUAGES CXX)

# --- มาตรฐานภาษา ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- ดึง FTXUI จาก GitHub อัตโนมัติ (ต้องมีเน็ตตอน configure ครั้งแรก) ---
include(FetchContent)
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG        v5.0.0          # ล็อกเวอร์ชันไว้ กัน API เปลี่ยน
)
FetchContent_MakeAvailable(ftxui)

# --- รวมไฟล์ .cpp ทั้งหมดของเกม ---
add_executable(TetrisTerminal
    main.cpp
    models/Tetromino.cpp
    models/Board.cpp
    models/Game.cpp
    handlers/PieceGenerator.cpp
    handlers/LevelManager.cpp
    handlers/Renderer.cpp
)

# --- ให้ทุกไฟล์หา header ข้าม folder เจอ (models/ กับ handlers/) ---
# เพราะ handlers/*.h include "Types.h" (อยู่ใน models/) และ Game.h include ข้าม folder
target_include_directories(TetrisTerminal PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/handlers
)

# --- ลิงก์ FTXUI (3 ส่วน: component ต้องมาก่อน dom ก่อน screen) ---
target_link_libraries(TetrisTerminal PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
)