#include <math.h>
#include <raylib.h>

typedef struct Ball {
    float cx, cy;
    float radius;
    Color color;

    int sx, sy;
} Ball;

typedef struct Paddle {
    float x, y;
    float w, h;
    Color color;

    int s;
} Paddle;

void makeBall(Ball* b, float cx, float cy, float r, Color c);
void drawBall(Ball* b);
void updateBall(Ball* b);

void makePaddle(Paddle* p, float x, float y, float w, float h, Color c);
void drawPaddle(Paddle* p);
void updatePaddle(Paddle* p);

void updateAI(Paddle* p, Ball* b);

int main(void) {
    const int windowWidth = 800;
    const int windowHeight = 450;

    const char* windowName = "Pong";

    const int paddleWidth = 20;
    const int paddleHeight = 120;

    Ball ball = {0};
    Paddle player = {0};
    Paddle ai = {0};

    makeBall(&ball, windowWidth*0.5, windowHeight*0.5, 10, RED);
    makePaddle(&player, windowWidth - paddleWidth - 10, windowHeight*0.5 - paddleHeight*0.5, paddleWidth, paddleHeight, RED);
    makePaddle(&ai, 0 + 10, windowHeight*0.5 - paddleHeight*0.5, paddleWidth, paddleHeight, RED);

    InitWindow(windowWidth, windowHeight, windowName);
    SetTargetFPS(60);

    #define BACKGROUND CLITERAL(Color){ 24, 24, 24, 255 }

    while ( !WindowShouldClose() ) {
        BeginDrawing();
            updateBall(&ball);
            updatePaddle(&player);
            updateAI(&ai, &ball);

            ClearBackground(BACKGROUND);

            DrawLine(windowWidth/2, 0, windowWidth/2, windowWidth, WHITE);
            drawBall(&ball);
            drawPaddle(&player);
            drawPaddle(&ai);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


void makeBall(Ball* b, float cx, float cy, float r, Color c) {
    b->cx = cx;
    b->cy = cy;
    b->radius = r;
    b->color = c;
    b->sx = 4;
    b->sy = 4;
}

void drawBall(Ball* b) {
    DrawCircle((int)ceilf(b->cx), (int)ceilf(b->cy), b->radius, b->color);
}

void updateBall(Ball* b) {
    b->cx += b->sx;
    b->cy += b->sy;

    // Change direction upon colliding with the walls
    if (b->cx+b->radius >= GetScreenWidth() || b->cx - b->radius <= 0) {
        b->sx *= -1;
    }
    if (b->cy+b->radius >= GetScreenHeight() || b->cy - b->radius <= 0) {
        b->sy *= -1;
    }
}

void makePaddle(Paddle* p, float x, float y, float w, float h, Color c) {
    p->x = x;
    p->y = y;
    p->w = w;
    p->h = h;
    p->color = c;
    p->s = 4;
}

void drawPaddle(Paddle* p) {
    DrawRectangle(p->x, p->y, p->w, p->h, p->color);
}

void _bounds_check(float* y, float* h);

void updatePaddle(Paddle* p) {
    if (IsKeyDown(KEY_UP)) {
        p->y -= p->s;
    }
    if (IsKeyDown(KEY_DOWN)) {
        p->y += p->s;
    }
    _bounds_check(&p->y, &p->h);
}

void _bounds_check(float* y, float* h) {
    // Bounds check
    if (*y <= 0) {
        *y = 0;
    }
    if (*y + *h >= GetScreenHeight()) {
        *y = GetScreenHeight() - *h;
    }
}

void updateAI(Paddle* p, Ball* b) {
    if (p->y + p->h/2 > b->cy) {
        p->y -= p->s;
    }
    if (p->y + p->h/2 <= b->cy) {
        p->y += p->s;
    }
    _bounds_check(&p->y, &p->h);
}
