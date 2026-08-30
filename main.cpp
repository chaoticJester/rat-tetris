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
    float lockTimer = 0;          
    int lockResets = 0;                 // นับจำนวนครั้งที่สไลด์/หมุน ตอนแตะพื้น
    int lowestY = 0;                    // เก็บตำแหน่ง Y ที่ต่ำที่สุดที่ชิ้นส่วนเคยลงไปถึง
    const int MAX_RESETS = 15;          // จำนวนครั้งสูงสุดที่อนุญาตให้ยื้อได้

    // Lambda function สำหรับหาจุดต่ำสุดของบล็อกชิ้นปัจจุบัน
    auto getPieceBottomY = [&]() {
        int bottomY = -1;
        for (const auto& b : game.getCurrentBlocks()) {
            if (b.y > bottomY) bottomY = b.y;
            }
            return bottomY;
    };

    lowestY = getPieceBottomY(); // กำหนดค่าเริ่มต้น
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
                // ประกาศไว้ก่อนเริ่มเช็ก Input
                bool actionSuccess = false; 

                if (IsKeyPressed(KEY_LEFT)) {          
                    if (game.moveLeft()) actionSuccess = true; 
                    dasTimer = 0;                       
                }
                else if (IsKeyDown(KEY_LEFT)) {        
                    dasTimer += GetFrameTime();
                    if (dasTimer >= DAS) {             
                        if (game.moveLeft()) actionSuccess = true;               
                        dasTimer -= ARR;               
                    }
                }
                if (IsKeyPressed(KEY_RIGHT)) {          
                    if (game.moveRight()) actionSuccess = true;                   
                    dasTimer = 0;                       
                }
                else if (IsKeyDown(KEY_RIGHT)) {        
                    dasTimer += GetFrameTime();
                    if (dasTimer >= DAS) {             
                        if (game.moveRight()) actionSuccess = true;               
                        dasTimer -= ARR;            
                    }
                }
                if (IsKeyPressed(KEY_X)) {
                    if (game.rotate(Direction::CW)) actionSuccess = true;
                }
                if (IsKeyPressed(KEY_Z)) {
                    if (game.rotate(Direction::CCW)) actionSuccess = true;
                }

                // โค้ดส่วน Soft Drop ใช้ของเดิมได้เลย
                if (IsKeyDown(KEY_DOWN)) { 
                    softDropTimer += GetFrameTime();
                    if (softDropTimer >= 0.05f) {   
                        game.softDrop();
                        softDropTimer = 0;
                    }
                } else {
                    softDropTimer = 0;
                }

                // รีเซ็ตค่าหากมีการ Hard Drop (เพราะเกิดบล็อกชิ้นใหม่)
                if (IsKeyPressed(KEY_SPACE)) {
                    game.hardDrop();
                    lockTimer = 0.0f; fallTimer = 0.0f;
                    lockResets = 0; lowestY = getPieceBottomY();
                }
                // รีเซ็ตค่าหากมีการ Hold (เพราะเกิดบล็อกชิ้นใหม่)
                if (IsKeyPressed(KEY_C) || IsKeyPressed(KEY_LEFT_SHIFT)) {
                    if (game.hold()) {
                        lockTimer = 0.0f; fallTimer = 0.0f;
                        lockResets = 0; lowestY = getPieceBottomY();
                    }
                }

                // --- ระบบ Reset Timer (Infinity Rule) ---
                // 1. ตรวจสอบถ้าบล็อกหล่นลงมาลึกกว่าเดิม ให้เคลียร์โควต้าการขยับ
                int currentBottom = getPieceBottomY();
                if (currentBottom > lowestY) {
                    lowestY = currentBottom;
                    lockResets = 0; 
                }

                // 2. ถ้าผู้เล่นขยับ/หมุนสำเร็จ ตอนที่บล็อกแตะพื้นไปแล้ว
                if (actionSuccess) {
                    // ถ้ายื้อไม่เกินลิมิต ให้รีเซ็ตเวลา
                    if (!game.canMoveDown() && lockResets < MAX_RESETS) {
                        lockTimer = 0.0f; 
                        lockResets++;     
                    }
                }

                // ========== UPDATE (gravity & lock delay) ==========
                if (!game.canMoveDown()) {
                    lockTimer += GetFrameTime();
                    
                    // ถ้าเวลาเกิน Lock Delay (หรือเกินโควต้า 15 ครั้งแล้วไม่ขยับต่อ) ให้ล็อก
                    if (lockTimer >= game.getLockDelay()) {
                        game.lock();
                        lockTimer = 0.0f;  
                        fallTimer = 0.0f;
                        
                        // --- ส่วนที่เพิ่มเข้ามา ---
                        lockResets = 0;                 // เริ่มนับโควต้าใหม่
                        lowestY = getPieceBottomY();    // เก็บความลึกของบล็อกชิ้นใหม่
                    }
                } else {
                    lockTimer = 0.0f; 
                    fallTimer += GetFrameTime();              
                    float delay = game.getFallDelay();
                    if (delay <= 0) delay = 0.05f;            
                    
                    if (fallTimer >= delay) {
                        game.softDrop();   
                        fallTimer = 0.0f;                         
                    }
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
