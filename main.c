#include <raylib.h>

typedef struct Ball {
    int cx, cy;
    float radius;
    Color color;

    int sx, sy;
} Ball;

void makeBall(Ball* b, int cx, int cy, float r, Color c);
void drawBall(Ball* b);
void updateBall(Ball* b);

int main(void) {
    const int windowWidth = 800;
    const int windowHeight = 450;
    const char* windowName = "Pong";

    const int paddleWidth = 20;
    const int paddleHeight = 120;

    Ball ball = {0};
    makeBall(&ball, windowWidth/2, windowHeight/2, 10, RED);

    InitWindow(windowWidth, windowHeight, windowName);
    SetTargetFPS(60);

    #define BACKGROUND CLITERAL(Color){ 24, 24, 24, 255 }

    while ( !WindowShouldClose() ) {
        BeginDrawing();
            updateBall(&ball);

            ClearBackground(BACKGROUND);

            DrawLine(windowWidth/2, 0, windowWidth/2, windowWidth, WHITE);
            drawBall(&ball);
            DrawRectangle(0 + 10, windowHeight/2 - paddleHeight/2, paddleWidth, paddleHeight, RED);
            DrawRectangle(windowWidth - paddleWidth - 10, windowHeight/2 - paddleHeight/2, paddleWidth, paddleHeight, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


void makeBall(Ball* b, int cx, int cy, float r, Color c) {
    b->cx = cx; b->cy = cy;
    b->radius = r;
    b->color = c;
    b->sx = 4; b->sy = 4;
}

void drawBall(Ball* b) {
    DrawCircle(b->cx, b->cy, b->radius, b->color);
}

void updateBall(Ball* b) {
    b->cx += b->sx;
    b->cy += b->sy;

    if (b->cx+b->radius >= GetScreenWidth() || b->cx - b->radius <= 0) {
        b->sx *= -1;
    }
    if (b->cy+b->radius >= GetScreenHeight() || b->cy - b->radius <= 0) {
        b->sy *= -1;
    }
}
