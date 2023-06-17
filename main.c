#include <raylib.h>

int main(void) {
    const int windowWidth = 800;
    const int windowHeight = 450;
    const char* windowName = "Pong";

    const int paddleWidth = 20;
    const int paddleHeight = 120;

    InitWindow(windowWidth, windowHeight, windowName);
    SetTargetFPS(60);

    #define BACKGROUND CLITERAL(Color){ 24, 24, 24, 255 }

    while ( !WindowShouldClose() ) {
        BeginDrawing();
            ClearBackground(BACKGROUND);
            DrawLine(windowWidth/2, 0, windowWidth/2, windowWidth, WHITE);
            DrawCircle(windowWidth/2, windowHeight/2, 10, RED);
            DrawRectangle(0 + 10, windowHeight/2 - paddleHeight/2, paddleWidth, paddleHeight, RED);
            DrawRectangle(windowWidth - paddleWidth - 10, windowHeight/2 - paddleHeight/2, paddleWidth, paddleHeight, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
