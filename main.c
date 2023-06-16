#include <raylib.h>

int main(void) {
    const int windowWidth = 800;
    const int windowHeight = 450;
    const char* windowName = "Pong";

    InitWindow(windowWidth, windowHeight, windowName);
    SetTargetFPS(60);

    while ( !WindowShouldClose() ) {
        BeginDrawing();
            ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
