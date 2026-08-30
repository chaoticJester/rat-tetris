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
    Sound gameOver = LoadSound("sounds/sfx/game-over-voice.mp3");
    int lastLines = game.getTotalLines();
    int lastLocks = game.getLockCount();

    
    int currentLevel = game.getLevel();
    Music currentMusic = LoadMusicStream(musicLibrary[0].c_str());
    PlayMusicStream(currentMusic);
    
    float fallTimer = 0; 
    float softDropTimer = 0;  // สำหรับลง
    float dasTimer = 0;           // นับเวลาที่กดค้าง
    const float DAS = 0.15f;      // หน่วงก่อนเริ่มรัว (วินาที)
    const float ARR = 0.03f;      // ความเร็วรัว (วินาที/ช่อง)
    bool gameOverHandled = false;

    // ประกาศสถานะของเกม
    enum class GameState { PLAYING, TRANSITION, GAME_OVER };
    GameState state = GameState::PLAYING;
    float transitionTimer = 0.0f; // ตัวจับเวลาหน้าคั่นเลเวล

    while (!WindowShouldClose()) {
        
        // ========== อัปเดตเสียง (ทำทุกเฟรม) ==========
        UpdateMusicStream(currentMusic);

        // ========== จัดการสถานะเกม ==========
        if (state == GameState::PLAYING) {
            
            // 1. เช็คเกมโอเวอร์
            if (game.isGameOver()) {
                state = GameState::GAME_OVER;
                gameOverHandled = false;
            } 
            // 2. เช็คเลเวลอัป
            else if (currentLevel != game.getLevel()) {
                currentLevel = game.getLevel();
                state = GameState::TRANSITION;
                transitionTimer = 2.0f; // ตั้งเวลาแสดงหน้าคั่น 2 วินาที (ปรับได้)
                
                // เปลี่ยนเพลง
                UnloadMusicStream(currentMusic); 
                if(currentLevel < 20) {
                    currentMusic = LoadMusicStream(musicLibrary[currentLevel - 1].c_str());
                } else {
                    currentMusic = LoadMusicStream(musicLibrary[19].c_str());
                }
                
                PlayMusicStream(currentMusic);  
            } else {

                // 3. ลอจิกตอนเล่นปกติ (ใส่ของเดิมไว้ตรงนี้)
                
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
            }

        } else if (state == GameState::TRANSITION) {
            // นับเวลาถอยหลังหน้าคั่น
            transitionTimer -= GetFrameTime();
            
            // ถ้าหมดเวลา ให้กลับไปเล่นต่อ
            if (transitionTimer <= 0.0f) {
                state = GameState::PLAYING; 
            }
        } else if (state == GameState::GAME_OVER) {
            
            if (!gameOverHandled) {
                gameOverHandled = true;
                UnloadMusicStream(currentMusic);      
                PlaySound(gameOver);                  
                currentMusic = LoadMusicStream("sounds/music/game_over.mp3");
                PlayMusicStream(currentMusic); 
            }
            
            if (IsKeyPressed(KEY_R)) {
                game = Game();
                lastLocks = game.getLockCount();
                lastLines = game.getTotalLines();
                currentLevel = game.getLevel();                          
                
                UnloadMusicStream(currentMusic);      
                currentMusic = LoadMusicStream(musicLibrary[0].c_str()); 
                PlayMusicStream(currentMusic);
                
                gameOverHandled = false;
                state = GameState::PLAYING; // รีเซ็ตสถานะกลับมาเล่น
            }
        }

        // ========== DRAW ==========
        BeginDrawing();
        ClearBackground(BLACK);
        
        // วาดกระดานเกมเสมอ ไม่ว่าจะอยู่สถานะไหน
        renderer.render(game);

        if (state == GameState::TRANSITION) {
            // วาดจอ LEVEL UP ทับ (ดำโปร่งใส)
            DrawRectangle(0, 0, 1280, 720, Color{0, 0, 0, 180});   
            
            const char* levelText = TextFormat("LEVEL %d", currentLevel);
            int textWidth = MeasureText(levelText, 80);
            
            // วาดข้อความเลเวลอัปตรงกลางจอ
            DrawText(levelText, 640 - (textWidth / 2), 300, 80, GOLD);
            
            // เพิ่มข้อความเตรียมตัว
            int readyWidth = MeasureText("GET READY...", 30);
            DrawText("GET READY...", 640 - (readyWidth / 2), 400, 30, RAYWHITE);
        }
        else if (state == GameState::GAME_OVER) {
            // วาดจอ GAME OVER ทับ
            DrawRectangle(0, 0, 1280, 720, Color{0, 0, 0, 180});   
            
            int titleWidth = MeasureText("GAME OVER", 60);
            int subWidth = MeasureText("Press R to restart", 30);
            
            DrawText("GAME OVER", 640 - (titleWidth / 2), 300, 60, RED);
            DrawText("Press R to restart", 640 - (subWidth / 2), 380, 30, RAYWHITE);
        }

        EndDrawing();
    }
    CloseAudioDevice(); 
    CloseWindow();
    return 0;
}
