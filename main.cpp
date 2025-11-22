#include <iostream>
#include <raylib.h>
#include <time.h>
using namespace std;
int playerScore = 0;
int playerLives = 3;
struct bullets
{
    float radius;
    float xBullet, yBullet;
    int bulletSpeedX, bulletSpeedY;
    void Draw()
    {
        DrawCircle(xBullet, yBullet, radius, WHITE);
    }
    void Update()
    {
        yBullet -= bulletSpeedY; // moves up
    }
};
struct spaceShip
{
    // attribute or qualities
    float xShip, yShip;
    float widthShip, heightShip;
    int speedShip; // only for y axis as only up down
    void Draw()
    { // functions inside structs are called methods
        DrawRectangle(xShip, yShip, widthShip, heightShip, WHITE);
    }
    void Update(bullets bullet[], int &bulletCount)
    {
        if (IsKeyDown(KEY_RIGHT))
        {
            xShip = xShip + speedShip;
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            xShip = xShip - speedShip;
        }
        // for not moving out of the upper bound
        if (xShip <= 0)
            xShip = 0;
        else if (xShip + widthShip >= GetScreenWidth())
            xShip = GetScreenWidth() - widthShip;

        // for bullets
        if (IsKeyPressed(KEY_SPACE))
        {
            bullet[bulletCount].xBullet = xShip + widthShip / 2;
            bullet[bulletCount].yBullet = yShip;
            bullet[bulletCount].bulletSpeedY = 15;
            bullet[bulletCount].radius = 5;
            bulletCount++;
            if (bulletCount >= 100)
                bulletCount = 0; // dubara zero se start
        }
    }
};
struct enemy
{
    float xEnemy, yEnemy;
    float widthEnemy, heightEnemy;
    int speed;
    void Draw()
    {
        DrawRectangle(xEnemy, yEnemy, widthEnemy, heightEnemy, WHITE);
    }
    void Update()
    {
        yEnemy += speed;                // moves down
        if (yEnemy > GetScreenHeight()) // for respawning
        {
            yEnemy = -30;
            xEnemy = rand() % +(GetScreenWidth() - 20);
            playerLives -= 1;
        }
        // if ((xEnemy + heightEnemy) >= GetScreenHeight()) // cpu wins
    }
};
void setAsteroids(enemy a[], int size)
{
    srand(time(0));
    // interacting with the elements of the enemy
    for (int i = 0; i < size; i++)
    {
        a[i].widthEnemy = (rand() % 21) + 15;
        a[i].heightEnemy = (rand() % 21) + 15;
        // so that they spawn randomly
        a[i].xEnemy = rand() % (GetScreenWidth() - 10);
        a[i].yEnemy = rand() % (GetScreenHeight() - 700);
        a[i].speed = 1;
    }
};
spaceShip ship;
enemy asteroid[3];
bullets bullet[100];
int bulletCount = 0;

int main()
{
    const int screenWidth = 1000;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Space Shooter");
    SetTargetFPS(60);
    // initializations or interaction with the elements
    ship.widthShip = 50;
    ship.heightShip = 25;
    ship.xShip = screenWidth / 2 - ship.widthShip / 2; // at horizontal center
    ship.yShip = screenHeight - ship.heightShip - 10;  // Bottom of screen
    ship.speedShip = 9;

    setAsteroids(asteroid, 3);

    // game loop
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        ship.Update(bullet, bulletCount);

        ship.Draw();
        for (int i = 0; i < 3; i++)
        {
            asteroid[i].Update();
            asteroid[i].Draw();
        }
        for (int i = 0; i < 100; i++)
        {
            bullet[i].Update();
            bullet[i].Draw();
        }
        for (int b = 0; b < 100; b++)
        {
            if (bullet[b].bulletSpeedY > 0)
            { // checking for active bullets
                for (int a = 0; a < 3; a++)
                {
                    if (CheckCollisionCircleRec(Vector2{bullet[b].xBullet, bullet[b].yBullet}, bullet[b].radius, Rectangle{asteroid[a].xEnemy, asteroid[a].yEnemy, asteroid[a].widthEnemy, asteroid[a].heightEnemy}))
                    //                           from this           bullet center                       radius
                    { // for removal
                        asteroid[a].yEnemy = -100;
                        bullet[b].yBullet = -100;

                        bullet[b].bulletSpeedY = 0;//stop bullet

                        asteroid[a].xEnemy = rand() % (GetScreenWidth() - 20);

                        playerScore += 10;
                    }
                }
            }
        }
        if (playerLives <= 0)
            DrawText("GAME OVER", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2, 40, WHITE);

        DrawText(TextFormat("SCORE: %d", playerScore), 10, 10, 20, WHITE);
        DrawText(TextFormat("LIVES: %d", playerLives), 10, 40, 20, WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}