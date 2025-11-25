#include <iostream>
#include <raylib.h>
#include <time.h>
using namespace std;
const int maxBullets = 100;
const int maxAsteroids = 7;
const int maxLevel = 5; 
const int perLevelScore = 100;
const int screenWidth = 900;
const int screenHeight = 700;
int messageFrames = 0;
int messageLevel = 0;
float messageAlpha = 1.0;
int previousLevel = 1;

const int menuState = 0;
const int playingState = 1;
const int gameOverState = 2;
const int gameWonState = 3;

int currentState = menuState;

int gameMode = 0; 
int currentLevel = 1;
int activeAsteroids = 2;
float baseEnemySpeed = 2.0;

int playerScore = 0;
int playerLives = 5;
struct Star
{
    float x, y;
    float speed;
};

struct bullets
{
    float radius;
    float xBullet, yBullet;
    int bulletSpeedX, bulletSpeedY;
    void Draw()
    {
        if (bulletSpeedY > 0)
            DrawCircle(xBullet, yBullet, radius, WHITE);
    }
    void Update()
    {
        if (bulletSpeedY > 0)
            yBullet -= bulletSpeedY;

        if (yBullet < 0)
        {
            bulletSpeedY = 0;
            yBullet = -20;
        }
    }
};
struct spaceShip
{
    // attribute or qualities
    float xShip, yShip;
    float widthShip, heightShip;
    int speedShip;
    void Draw()
    { // functions inside structs are called methods
        DrawRectangle(xShip, yShip, widthShip, heightShip, WHITE);
    }
    void Update(bullets bullet[])
    {
        if (IsKeyDown(KEY_RIGHT))
            xShip = xShip + speedShip;

        else if (IsKeyDown(KEY_LEFT))
            xShip = xShip - speedShip;

        if (xShip <= 0)
            xShip = 0;
        else if (xShip + widthShip >= GetScreenWidth())
            xShip = GetScreenWidth() - widthShip;

        // Bullet Firing Input
        if (IsKeyPressed(KEY_SPACE))
        {
            int shots = 1;
            if (currentLevel == 3 || currentLevel == 4)
                shots = 2;
            if (currentLevel == 5)
                shots = 3;

            float centerX = xShip + widthShip / 2.0;
            float offset = 20.0;

            int fired = 0;
            for (int i = 0; i < maxBullets && fired < shots; i++)
            {
                if (bullet[i].bulletSpeedY == 0)
                {
                    float xPosition = centerX + (fired - (shots / 2.0f)) * offset;

                    bullet[i].xBullet = xPosition;
                    bullet[i].yBullet = yShip; 
                    bullet[i].radius = 5;
                    bullet[i].bulletSpeedY = 15;

                    fired++;
                }
            }
        }
    }
};
struct enemy
{
    float xEnemy, yEnemy;
    float widthEnemy, heightEnemy;
    float yVelocity;
    float xVelocity;

    void Draw()
    {
        DrawRectangle(xEnemy, yEnemy, widthEnemy, heightEnemy, WHITE);
    }
    void Update()
    {
        yEnemy += yVelocity;
        xEnemy += xVelocity;
        if (xEnemy <= 0)
        {
            xEnemy = 0;
            xVelocity = (xVelocity < 0) ? -xVelocity : xVelocity;
        }
        else if (xEnemy + widthEnemy >= screenWidth)
        {
            xEnemy = screenWidth - widthEnemy;
            xVelocity = (xVelocity > 0) ? -xVelocity : xVelocity;
        }

        if (yEnemy > GetScreenHeight())
        {
            yEnemy = -50 - GetRandomValue(0, 150);
            xEnemy = GetRandomValue(0, screenWidth - (int)widthEnemy);
            xVelocity = GetRandomValue(1, 2) * (GetRandomValue(0, 1) ? 1 : -1);
            playerLives -= 1;
        }
    }
};
void setEnemies(enemy a[], int maxAsteroids, float speedValue)
{
    for (int i = 0; i < maxAsteroids; i++)
    {
        a[i].widthEnemy = 25;
        a[i].heightEnemy = 25;

        a[i].xEnemy = rand() % (GetScreenWidth() - 10);
        a[i].yEnemy = -50 - (rand() % 100);
        a[i].yVelocity = speedValue;
        a[i].xVelocity = GetRandomValue(-2, 2);
    }
};

Star stars[100];
spaceShip ship;
enemy asteroid[7];
bullets bullet[100];
int bulletCount = 0;
void resetGame(int screenWidth, int screenHeight)
{
    playerScore = 0;
    playerLives = 5;
    currentLevel = 1;
    bulletCount = 0;

    // Reset ship position
    ship.xShip = screenWidth / 2 - ship.widthShip / 2;
    ship.yShip = screenHeight - ship.heightShip - 10;

    // Reset all bullets
    for (int i = 0; i < maxBullets; i++)
    {
        bullet[i].bulletSpeedY = 0;
        bullet[i].yBullet = -100;
    }
}
void StartGame(int mode, int asteroidsCount, float speed)
{
    gameMode = mode;
    activeAsteroids = asteroidsCount;
    baseEnemySpeed = speed;

    resetGame(screenWidth, screenHeight);
    setEnemies(asteroid, activeAsteroids, baseEnemySpeed);

    currentState = playingState;
}

int main()
{
    srand(time(0));
    InitWindow(screenWidth, screenHeight, "Space Shooter");
    SetTargetFPS(60);

    ship.widthShip = 55;
    ship.heightShip = 25;
    ship.xShip = screenWidth / 2 - ship.widthShip / 2;
    ship.yShip = screenHeight - ship.heightShip - 10;
    ship.speedShip = 10;

    setEnemies(asteroid, activeAsteroids, 2);

    // Initialize stars (simple background)
    for (int i = 0; i < 100; i++)
    {
        stars[i].x = (float)(rand() % screenWidth);
        stars[i].y = (float)(rand() % screenHeight);
        stars[i].speed = 1 + (rand() % 3); 
    }

    // game loop
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        // Emergency exit
        if (IsKeyPressed(KEY_Q))
        {
            break;
        }

        if (currentState == menuState)
        {
            DrawText("SPACE SHOOTER", screenWidth / 2 - 150, 100, 40, YELLOW);
            DrawText("Select Difficulty:", screenWidth / 2 - 100, 200, 30, YELLOW);
            DrawText("1 - EASY", screenWidth / 2 - 80, 250, 20, YELLOW);
            DrawText("2 - MEDIUM", screenWidth / 2 - 80, 280, 20, YELLOW);
            DrawText("3 - HARD", screenWidth / 2 - 80, 310, 20, YELLOW);
            DrawText("Objective: Destroy enemies & survive.", 250, 360, 20, LIGHTGRAY);
            DrawText("Controls: Left and Right Arrow Keys, SPACE Shoot, Q Quit", 200, 390, 20, LIGHTGRAY);
            DrawText("There are 5 levels, at level 3 you get DoubleShot, at level 5 you get TripleShot", 50, 420, 20, LIGHTGRAY);
            DrawText("To WIN you have to clear all five levels ", 250, 450, 20, LIGHTGRAY);
            DrawText("GOOD LUCK ! ", 375, 480, 20, LIGHTGRAY);

            if (IsKeyPressed(KEY_ONE))
                StartGame(0, 2, 1.5f);
            if (IsKeyPressed(KEY_TWO))
                StartGame(1, 3, 2.0f);
            if (IsKeyPressed(KEY_THREE))
                StartGame(2, 4, 2.5f);
        }
        else if (currentState == playingState)
        {
            // create stars
            for (int i = 0; i < 100; i++)
            {
                stars[i].y += stars[i].speed;

                if (stars[i].y > screenHeight)
                {
                    stars[i].x = rand() % screenWidth;
                    stars[i].y = -10;
                }

                DrawPixel(stars[i].x, stars[i].y, WHITE);
            }

            ship.Update(bullet);
            for (int i = 0; i < activeAsteroids; i++)
            {
                asteroid[i].Update();
            }
            for (int i = 0; i < maxBullets; i++)
            {
                bullet[i].Update();
            }
            
            for (int i = 0; i < maxBullets; i++)
            {
                if (bullet[i].bulletSpeedY <= 0)
                    continue;

                for (int j = 0; j < activeAsteroids; j++)
                {
                    if (CheckCollisionCircleRec(Vector2{bullet[i].xBullet, bullet[i].yBullet}, bullet[i].radius, Rectangle{asteroid[j].xEnemy, asteroid[j].yEnemy, asteroid[j].widthEnemy, asteroid[j].heightEnemy}))
                    {
                        bullet[i].bulletSpeedY = 0;
                        bullet[i].yBullet = -20;

                        asteroid[j].yEnemy = -100;
                        asteroid[j].xEnemy = rand() % (GetScreenWidth() - 20);

                        playerScore += 10;
                    }
                }
            }
            for (int j = 0; j < activeAsteroids; j++)
            {
                if (CheckCollisionRecs(Rectangle{ship.xShip, ship.yShip, ship.widthShip, ship.heightShip}, Rectangle{asteroid[j].xEnemy, asteroid[j].yEnemy, asteroid[j].widthEnemy, asteroid[j].heightEnemy}))
                {
                    playerLives -= 3;
                    asteroid[j].yEnemy = -100;
                }
            }

            int targetScoreForEachLevel = currentLevel * perLevelScore;

            if (playerScore >= targetScoreForEachLevel)
            {
                if (currentLevel < maxLevel)
                {
                    currentLevel++;
                    messageLevel = currentLevel;
                    messageFrames = 120;
                    messageAlpha = 1.0;
                    previousLevel = currentLevel;

                    if (currentLevel == 3)
                        playerLives += 1;

                    float speedMultiplier = 1.0 + (currentLevel * 0.2);
                    int countIncrease = currentLevel / 2;
                    float newSpeed = baseEnemySpeed * speedMultiplier;
                    int newActiveCount = activeAsteroids + countIncrease;

                    if (newActiveCount > maxAsteroids)
                        newActiveCount = maxAsteroids;
                    if (newSpeed > 8.0)
                        newSpeed = 8.0;

                    activeAsteroids = newActiveCount;

                    // Re-initialize enemies for the new level
                    setEnemies(asteroid, activeAsteroids, newSpeed);
                }
                else
                {
                    currentState = gameWonState;
                }
            }
            if (playerLives <= 0)
            {
                currentState = gameOverState;
            }
            ship.Draw();
            for (int i = 0; i < activeAsteroids; i++)
            {
                asteroid[i].Draw();
            }
            for (int i = 0; i < maxBullets; i++)
            {
                bullet[i].Draw();
            }
            DrawText(TextFormat("SCORE: %d", playerScore), 10, 10, 20, WHITE);
            DrawText(TextFormat("LIVES: %d", playerLives), 10, 40, 20, WHITE);
            DrawText(TextFormat("LEVEL: %d", currentLevel), 10, 70, 20, WHITE);
            if (messageFrames > 0)
            {
                messageFrames--;

                // message fade per frame
                messageAlpha = messageAlpha - 0.01;
                if (messageAlpha < 0)
                    messageAlpha = 0;
            }
            if (messageFrames > 0)
            {
                if (messageLevel == 3)
                {
                    DrawText("LIFE POWER-UP & DOUBLE SHOT!", screenWidth / 2 - 220, screenHeight / 2 - 20, 25, Fade(GREEN, messageAlpha));
                }
                if (messageLevel == 5)
                {
                    DrawText("TRIPLE SHOT ACTIVATED!", screenWidth / 2 - 150, screenHeight / 2, 25, Fade(YELLOW, messageAlpha));
                }
            }
        }
        else if (currentState == gameOverState)
        {
            DrawText("GAME OVER", screenWidth / 2 - 100, screenHeight / 2 - 50, 40, RED);
            DrawText(TextFormat("Final Score: %d", playerScore), screenWidth / 2 - 120, screenHeight / 2, 30, WHITE);
            DrawText("Press M for Menu or Q to Quit", screenWidth / 2 - 150, screenHeight / 2 + 50, 20, WHITE);

            if (IsKeyPressed(KEY_M))
            {
                currentState = menuState;
            }
        }
        else if (currentState == gameWonState)
        {
            DrawText("YOU WIN !", screenWidth / 2 - 80, screenHeight / 2 - 50, 40, GREEN);
            DrawText(TextFormat("Final Score: %d", playerScore), screenWidth / 2 - 120, screenHeight / 2, 30, WHITE);
            DrawText("Press M for Menu or Q to Quit", screenWidth / 2 - 150, screenHeight / 2 + 50, 20, WHITE);

            if (IsKeyPressed(KEY_M))
            {
                currentState = menuState;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}