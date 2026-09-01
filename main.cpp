#include "raylib.h"
#include "modes/NormalMode.h"


int main() {
    SetTraceLogLevel(LOG_NONE);
    InitWindow(1280, 720 , "RatTetris");
    SetTargetFPS(60);
    InitAudioDevice(); 

    const char *title = "RAT TETRIS";
    const char *text2 = "PRESS ENTER OR CLICK TO START";
    float fontSize = 100.0f;
    float spacing = 2.0f;

    // 1. คำนวณความกว้างที่แท้จริงของ "RAT TETRIS" ที่รวม spacing แล้ว
    float totalTitleWidth = 0.0f;
    for (int i = 0; title[i] != '\0'; i++) {
        char temp[2] = { title[i], '\0' };
        totalTitleWidth += MeasureText(temp, (int)fontSize);
        if (title[i+1] != '\0') {
            totalTitleWidth += spacing; // ไม่บวก spacing หลังตัวอักษรสุดท้าย
        }
    }

    // 2. จัดกึ่งกลางโดยเอาครึ่งจอ (640) ลบด้วยครึ่งหนึ่งของความกว้างทั้งหมด
    float titleStartAt = 640.0f - (totalTitleWidth / 2.0f);
    
    // (ส่วน text2 คุณเขียนมาได้ตรงกลางแล้วครับ)
    float text2StartAt = 640.0f - (MeasureText(text2, 20) / 2.0f);

    Music titleMusic = LoadMusicStream("sounds/music/title_music.wav");
    PlayMusicStream(titleMusic);
    SetMusicVolume(titleMusic, 0.5f);

    while (!WindowShouldClose()) {
        UpdateMusicStream(titleMusic);
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw each letter with a shifting hue
        float offsetX = 0.0f;
        float time = (float)GetTime() * 2.0f; // Speed of the rainbow

        for (int i = 0; title[i] != '\0'; i++) {
            // Shift hue per character and over time
            float hue = (time * 50.0f + (float)i * 15.0f);
            if (hue > 360.0f) hue = fmodf(hue, 360.0f);

            Color c = ColorFromHSV(hue, 1.0f, 1.0f);

            // Draw a single character as a string
            char temp[2] = { title[i], '\0' };
            DrawText(temp, (int)(titleStartAt + offsetX), (int)200, (int)fontSize, c);

            // Advance the X offset based on character width
            offsetX += MeasureText(temp, (int)fontSize) + spacing;
        }
        DrawText(text2, text2StartAt , 400, 20, RAYWHITE);
        EndDrawing();

        if(IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) NormalMode();
    }

    NormalMode();
    CloseAudioDevice(); 

    CloseWindow();
    return 0;
}
