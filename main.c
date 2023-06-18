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

void drawMidLine();
void drawScores();

void makeBall(Ball* b, float cx, float cy, float r, Color c);
void drawBall(Ball* b);
void updateBall(Ball* b);
void resetBall(Ball* b);

void makePaddle(Paddle* p, float x, float y, float w, float h, Color c);
void drawPaddle(Paddle* p);
void updatePaddle(Paddle* p);

void updateAI(Paddle* p, Ball* b);

Rectangle _paddle2vec4(Paddle* p);
Vector2 _ball2vec2(Ball* b);

void _bounds_check(float* y, float* h);
void _updateScore(Ball* b, Paddle* p, Paddle* ai);
void _checkBallPaddleCollision(Ball* b, Paddle* p, Paddle* ai);

int _getHalfScreenWidth();
int _getHalfScreenHeight();
int _getQuarterScreenWidth();
int _getQuarterScreenHeight();
int _getPaddleWidth();
int _getPaddleHeight();
int _getHalfPaddleWidth();
int _getHalfPaddleHeight();
int _getBallRadius();
int _getPadding();
int _getFontSize();

int player_score = 0;
int ai_score = 0;

int main(void) {
    const int windowWidth = 800;
    const int windowHeight = 450;

    const char* windowName = "Pong";

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWidth, windowHeight, windowName);
    SetTargetFPS(60);

    Ball ball = {0};
    Paddle player = {0};
    Paddle ai = {0};

    makeBall(&ball, _getHalfScreenWidth(), _getHalfScreenHeight(), _getBallRadius(), RED);
    makePaddle(&player, GetScreenWidth() - _getPaddleWidth() - _getPadding(), _getHalfScreenHeight() - _getHalfPaddleHeight(), _getPaddleWidth(), _getPaddleHeight(), RED);
    makePaddle(&ai, _getPadding(), _getHalfScreenHeight() - _getHalfPaddleHeight(), _getPaddleWidth(), _getPaddleHeight(), RED);

    #define BACKGROUND CLITERAL(Color){ 24, 24, 24, 255 }

    while ( !WindowShouldClose() ) {
        _checkBallPaddleCollision(&ball, &player, &ai);
        _updateScore(&ball, &player, &ai);

        updateBall(&ball);
        updatePaddle(&player);
        updateAI(&ai, &ball);

        BeginDrawing();
            ClearBackground(BACKGROUND);

            drawMidLine();
            drawBall(&ball);
            drawPaddle(&player);
            drawPaddle(&ai);
            drawScores();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void drawMidLine() {
    DrawLine(_getHalfScreenWidth(), 0, _getHalfScreenWidth(), GetScreenHeight(), WHITE);
}

int _getFontPadding() {
    return ceilf(GetScreenHeight()*0.04444f);
}

void drawScores() {
    DrawText(TextFormat("%i", ai_score), _getQuarterScreenWidth() - _getFontPadding(), _getFontPadding(), _getFontSize(), WHITE);
    DrawText(TextFormat("%i", player_score), 3*_getQuarterScreenWidth() - _getFontPadding(), _getFontPadding(), _getFontSize(), WHITE);
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
    if (b->cy + b->radius >= GetScreenHeight() || b->cy - b->radius <= 0) {
        b->sy *= -1;
    }
}

void resetBall(Ball* b) {
    b->cx = _getHalfScreenWidth();
    b->cy = _getHalfScreenHeight();

    int speed_choice[2] = {-1, 1};
    b->sx *= speed_choice[GetRandomValue(0, 1)];
    b->sy *= speed_choice[GetRandomValue(0, 1)];
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

void updatePaddle(Paddle* p) {
    if (IsWindowResized()) {
        p->x = GetScreenWidth() - _getPaddleWidth() - _getPadding();
    }

    if (IsKeyDown(KEY_UP)) {
        p->y -= p->s;
    }
    if (IsKeyDown(KEY_DOWN)) {
        p->y += p->s;
    }
    _bounds_check(&p->y, &p->h);
}

void resetPaddle(Paddle* player, Paddle* ai) {
    ai->x = _getPadding();
    ai->y = _getHalfScreenHeight() - _getHalfPaddleHeight();

    player->x = GetScreenWidth() - _getHalfPaddleWidth() - _getPadding();
    player->y = _getHalfScreenHeight() - _getHalfPaddleHeight();
}

void updateAI(Paddle* p, Ball* b) {
    if (IsWindowResized()) {
        p->x = _getPadding();
    }

    if (p->y + _getHalfPaddleHeight() > b->cy) {
        p->y -= p->s;
    }
    if (p->y + _getHalfPaddleHeight() <= b->cy) {
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

Rectangle _paddle2vec4(Paddle* p) {
    return (Rectangle) {p->x, p->y, p->w, p->h};
}

Vector2 _ball2vec2(Ball* b) {
    return (Vector2) {b->cx, b->cy};
}

void _updateScore(Ball* b, Paddle* p, Paddle* ai){
    if (b->cx + b->radius >= GetScreenWidth()) {
        ai_score++;
        resetBall(b);
        resetPaddle(p, ai);
        return;
    }
    if (b->cx - b->radius <= 0) {
        player_score++;
        resetBall(b);
        resetPaddle(p, ai);
        return;
    }
}

void _checkBallPaddleCollision(Ball* b, Paddle* p, Paddle* ai) {
    // Check if the Ball has collided with the paddle
    if (CheckCollisionCircleRec(_ball2vec2(b), b->radius, _paddle2vec4(p))) {
        b->sx *= -1;
    }
    if (CheckCollisionCircleRec(_ball2vec2(b), b->radius, _paddle2vec4(ai))) {
        b->sx *= -1;
    }
}

int _getHalfScreenWidth() {
    return GetScreenWidth()*0.5f;
}

int _getHalfScreenHeight() {
    return GetScreenHeight()*0.5f;
}

int _getQuarterScreenWidth() {
    return GetScreenWidth()*0.25f;
}

int _getQuarterScreenHeight() {
    return GetScreenHeight()*0.25f;
}

int _getPaddleWidth() {
    return ceilf(GetScreenWidth()*0.0125f);
}

int _getPaddleHeight() {
    return ceilf(GetScreenHeight()*0.26666666f);
}

int _getHalfPaddleWidth() {
    return ceilf(_getPaddleWidth()*0.5f);
}

int _getHalfPaddleHeight() {
    return ceilf(_getPaddleHeight()*0.5f);
}

int _getBallRadius() {
    return ceilf(GetScreenWidth()*0.0095f);
}

int _getPadding() {
    return ceilf(GetScreenWidth()*0.0125f);
}

int _getFontSize() {
    return ceilf(GetScreenWidth()*0.038f);
}
