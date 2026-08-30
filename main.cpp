#include "raylib.h"
#include "models/Game.h"
#include "handlers/Renderer.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


int main() {
    InitWindow(1280, 720 , "RatTetris");
    SetTargetFPS(60);
    InitAudioDevice(); 
    Game game;
    Renderer renderer;

    std::ifstream myFile("sounds/music/sound-list.txt");

    std::vector<std::string> musicLibrary;
    std::string line;
    while (std::getline(myFile, line)) {
        std::cout << line << std::endl;
        musicLibrary.push_back(line.c_str());
    }

    if (musicLibrary.empty()) {
        std::cout << "ไม่พบรายการเพลง!\n";
        // จัดการ เช่นข้ามระบบเพลง
    }   

    Sound lockSound  = LoadSound("sounds/sfx/lock.wav");
    Sound clearSound = LoadSound("sounds/sfx/clear.wav");
    int lastLines = game.getTotalLines();
    int lastLocks = game.getLockCount();

    float fallTimer = 0; 
    float softDropTimer = 0;  // สำหรับลง
    float dasTimer = 0;           // นับเวลาที่กดค้าง
    const float DAS = 0.15f;      // หน่วงก่อนเริ่มรัว (วินาที)
    const float ARR = 0.03f;      // ความเร็วรัว (วินาที/ช่อง)

    int currentLevel = game.getLevel();
    Music currentMusic = LoadMusicStream(musicLibrary[0].c_str());
    PlayMusicStream(currentMusic);


    while (!WindowShouldClose()) {
        // ========== BGM ==========
        if(currentLevel != game.getLevel()) {
            currentLevel= game.getLevel();
            UnloadMusicStream(currentMusic); 
            currentMusic = LoadMusicStream(musicLibrary[currentLevel % musicLibrary.size()].c_str());
            PlayMusicStream(currentMusic);  
        }
        if(!IsMusicStreamPlaying(currentMusic)) {
            PlayMusicStream(currentMusic);  
        }
        UpdateMusicStream(currentMusic);

        // ========== INPUT ==========
        if (IsKeyPressed(KEY_LEFT)) {          // กดครั้งแรก
            game.moveLeft();                    // เลื่อนทันที 1 ช่อง
            dasTimer = 0;                       // เริ่มนับ DAS
        }
        else if (IsKeyDown(KEY_LEFT)) {        // กดค้างต่อ
            dasTimer += GetFrameTime();
            if (dasTimer >= DAS) {             // ผ่าน DAS delay แล้ว
                game.moveLeft();               // เริ่มรัว
                dasTimer -= ARR;               // เว้นระยะ ARR ก่อนรัวครั้งถัดไป
            }
        }
        if (IsKeyPressed(KEY_RIGHT)) {          
            game.moveRight();                   
            dasTimer = 0;                       
        }
        else if (IsKeyDown(KEY_RIGHT)) {        
            dasTimer += GetFrameTime();
            if (dasTimer >= DAS) {             
                game.moveRight();               
                dasTimer -= ARR;            
            }
        }
        if (IsKeyPressed(KEY_X)) {
            game.rotate(Direction::CW);
        }
        if (IsKeyPressed(KEY_Z)) {
            game.rotate(Direction::CCW);
        }
        if (IsKeyDown(KEY_DOWN)) { 
            softDropTimer += GetFrameTime();
            if (softDropTimer >= 0.05f) {   
                game.softDrop();
                softDropTimer = 0;
            }
        } else {
            softDropTimer = 0;
        }
        if (IsKeyPressed(KEY_SPACE)) {
            game.hardDrop();
        }
        if (IsKeyPressed(KEY_C) || IsKeyPressed(KEY_LEFT_SHIFT)) {
            game.hold();
        }

        // ========== UPDATE (gravity) ==========

        // ในloop:
        fallTimer += GetFrameTime();              // เวลาที่ผ่านไปแต่ละเฟรม (วินาที)
        float delay = game.getFallDelay();
        if (delay <= 0) delay = 0.05f;             // ← เคส fallDelay=0 (level>20) ที่เลื่อนมาจาก Phase 5!
        if (fallTimer >= delay) {
            game.tick();                           // ชิ้นตกเอง 1 ก้าว
            fallTimer = 0;                         // reset
        }

        //========== SFX ==========
        if (game.getLockCount() > lastLocks) {
            PlaySound(lockSound);
            lastLocks = game.getLockCount();
        } else if (game.getTotalLines() > lastLines) {
            PlaySound(clearSound);
            lastLines = game.getTotalLines();
        }

        // ========== DRAW ==========
        BeginDrawing();
        ClearBackground(BLACK);
        
        renderer.render(game);

        EndDrawing();

    }
    CloseAudioDevice(); 
    CloseWindow();
    return 0;
}
