#include <iostream>
#include <raylib.h>
#include <time.h>
using namespace std;
int playerScore = 0;
int playerLives = 3;
struct spaceShip
{
    // attribute or qualities
    float x, y;
    float width, height;
    int speed; // only for y axis as only up down
    void Draw()
    { // functions inside structs are called methods
        DrawRectangle(x, y, width, height, WHITE);
    }
    void Update()
    {
        if (IsKeyDown(KEY_RIGHT))
        {
            x = x + speed; // as neeche positive y
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            x = x - speed; // as neeche positive y
        }
        if (x <= 0)
        { // for not moving out of the upper bound
            x = 0;
        }
        else if (x + width >= GetScreenWidth())
        {
            x = GetScreenWidth() - width;
        }
    }
};
struct enemy
{
    float x, y;
    float width, height;
    int speed;
    void Draw()
    {
        DrawRectangle(x, y, width, height, WHITE);
    }
    void Update()
    {
        y += speed; // moves down
        if (y > GetScreenHeight())//for respawning
        {
            y = -30;
            x = rand() % +(GetScreenWidth() - 20);
        }
    }
};
struct bullets
{
    float radius;
    float x, y;
    int speedX, speedY;
    void Draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }
    void Update()
    {
        y -= speedY; // moves up
    }
};
void setAsteroids(enemy a[], int size)
{
    srand(time(0));

    for (int i = 0; i < size; i++)
    {
        a[i].width = 20;
        a[i].height = 20;
        a[i].x = rand() % (GetScreenWidth() - 10);
        a[i].y = -100; // the height of rectangle starts from the top left of the rec
        a[i].speed = 3;
    }
};
spaceShip ship;
enemy asteroid[10];

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Space Shooter");
    SetTargetFPS(60);
    // initializations or interaction with the elements
    ship.width = 50;
    ship.height = 25;
    ship.x = screenHeight - ship.height - 10;
    ship.y = screenWidth / 2 - ship.width / 2; // the height of rectangle starts from the top left of the rec
    ship.speed = 8;

    setAsteroids(asteroid, 10);

    // game loop
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        ship.Update();
        ship.Draw();
        for (int i = 0; i < 10; i++)
        {
            asteroid[i].Update();
            asteroid[i].Draw();
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}