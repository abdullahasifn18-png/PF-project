#include "game.h"
#include <time.h>
// when we use rand()% 5, values get selected from 0-4
// floats instead of int for smoothness

int highScore[3] = {0, 0, 0};

Texture2D bulletTexture;
Texture2D enemyTexture;

int currentState = menuState;
int currentLevel = 1;
int gameMode = 0;
int activeGerms = 2;
float baseEnemySpeed = 2.0;
int messageLevel = 0;
int messageFrames = 0;
float messageAlpha = 0.0f;
int previousLevel = 1;

int playerScore = 0;
int playerLives = 5;

// definations

void LoadHighScore()
{
    ifstream file("highscore.txt");
    if (file.is_open())
    {
        for (int i = 0; i < 3; i++)
            file >> highScore[i];
        file.close();
    }
    else
    {
        for (int i = 0; i < 3; i++)
            highScore[i] = 0;
    }
}

void SaveHighScore()
{
    ofstream file("highscore.txt");
    for (int i = 0; i < 3; i++)
        file << highScore[i] << " ";
    file.close();
}

// functions inside structs are called methods
void bullets::Draw()
{
    if (bulletSpeedY > 0)
        DrawTexture(texture, xBullet - texture.width / 2, yBullet, WHITE);
}

void bullets::Update()
{
    if (bulletSpeedY > 0)
        yBullet -= bulletSpeedY;

    if (yBullet < 0)
    {
        bulletSpeedY = 0;
        yBullet = -20;
    }
}

void spaceShip::Draw()
{
    DrawTexture(texture, xShip, yShip, WHITE);
}

void spaceShip::Update(bullets bullet[])
{
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        xShip += speedShip;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        xShip -= speedShip;

    if (xShip < 0)
        xShip = 0;
    if (xShip + widthShip > GetScreenWidth())
        xShip = GetScreenWidth() - widthShip;

    if (IsKeyPressed(KEY_SPACE))
    {
        int shots=0;

        if (currentLevel == 3 || currentLevel == 4)
        {
            shots = 2; 
        }
        else if (currentLevel == 5)
        {
            shots = 3; 
        }
        else
        {
            shots = 1; 
        }
        int centerX = xShip + widthShip / 2;
        int offset = 20;

        int fired = 0;
        for (int i = 0; i < maxBullets && fired < shots; i++)
        {
            if (bullet[i].bulletSpeedY == 0)
            {
                int xPosition;
                if (shots == 1)
                {
                    xPosition = centerX;
                }
                else
                {
                    xPosition = centerX + (fired - (shots - 1) / 2) * offset;
                }
                bullet[i].xBullet = xPosition;
                bullet[i].yBullet = yShip;
                bullet[i].radius = 5;
                bullet[i].bulletSpeedY = 15;
                fired++;
            }
        }
    }
}

void enemy::Draw()
{
    DrawTexture(texture, xEnemy, yEnemy, WHITE);
}

void enemy::Update()
{
    yEnemy += (int)yVelocity;
    xEnemy += (int)xVelocity;

    if (xEnemy <= 0)
    {
        xEnemy = 0;
        if (xVelocity < 0)
            xVelocity = -xVelocity;
    }
    else if (xEnemy + widthEnemy >= screenWidth)
    {
        xEnemy = screenWidth - widthEnemy;
        if (xVelocity > 0)
            xVelocity = -xVelocity;
    }

    if (yEnemy > GetScreenHeight())
    {
        yEnemy = -50 - (rand() % 151);
        xEnemy = rand() % (screenWidth - widthEnemy + 1);
        int speed = 1 + rand() % 2;
        int direction = (rand() % 2 == 0) ? 1 : -1;
        xVelocity = speed * direction;

        playerLives -= 1;
    }
}

// Fixed: Added count parameter usage to avoid undefined errors
void setEnemies(enemy a[], int maxGermsCount, float speedValue, Texture2D tex)
{
    for (int i = 0; i < maxGermsCount; i++)
    {
        a[i].widthEnemy = tex.width;
        a[i].heightEnemy = tex.height;
        a[i].texture = tex;

        a[i].xEnemy = rand() % (GetScreenWidth() - a[i].widthEnemy + 1);
        a[i].yEnemy = -50 - (rand() % 101);
        a[i].xVelocity = -2 + rand() % 5;
        a[i].yVelocity = speedValue;
    }
}

// objects

Star stars[100];
spaceShip ship;
enemy germs[maxGerms];
bullets bullet[maxBullets];

void resetGame(int screenWidth, int screenHeight)
{
    playerScore = 0;
    playerLives = 5;
    currentLevel = 1;

    for (int i = 0; i < maxBullets; i++)
    {
        bullet[i].bulletSpeedY = 0;
        bullet[i].yBullet = -100;
    }
}

void StartGame(int mode, int germsCount, float speed)
{
    gameMode = mode;
    activeGerms = germsCount;
    baseEnemySpeed = speed;

    resetGame(screenWidth, screenHeight);
    setEnemies(germs, activeGerms, baseEnemySpeed, enemyTexture);

    currentState = playingState;
}

int main()
{
    srand(time(0));
    InitWindow(screenWidth, screenHeight, "Space Shooter");
    SetTargetFPS(60);
    InitAudioDevice();
    Sound explosionSound = LoadSound("Sounds/oggexplosion.ogg");

    // Load textures
    Image spaceshipImage = LoadImage("Graphics/ship3.png");
    ImageResize(&spaceshipImage, desiredShipWidth, desiredShipHeight);
    ship.texture = LoadTextureFromImage(spaceshipImage);
    UnloadImage(spaceshipImage);

    Image bulletImage = LoadImage("Graphics/laserBullet.png");
    ImageResize(&bulletImage, desiredBulletSize, desiredBulletSize);
    bulletTexture = LoadTextureFromImage(bulletImage);
    UnloadImage(bulletImage);

    Image enemyImage = LoadImage("Graphics/enemy4.png");
    ImageResize(&enemyImage, desiredEnemySize, desiredEnemySize);
    enemyTexture = LoadTextureFromImage(enemyImage);
    UnloadImage(enemyImage);

    // Initializing ship, bullets, germs and stars
    ship.widthShip = ship.texture.width;
    ship.heightShip = ship.texture.height;
    ship.xShip = screenWidth / 2 - ship.widthShip / 2;
    ship.yShip = screenHeight - ship.heightShip - 10;
    ship.speedShip = 10;

    for (int i = 0; i < maxBullets; i++)
    {
        bullet[i].texture = bulletTexture;
        bullet[i].radius = desiredBulletSize / 2;
    }

    setEnemies(germs, activeGerms, 2, enemyTexture);

    for (int i = 0; i < 100; i++)
    {
        stars[i].x = rand() % screenWidth;  // 0 to 899
        stars[i].y = rand() % screenHeight; // 0 to 699
        stars[i].speed = rand() % 3 + 1;    // 1 to 3
    }

    LoadHighScore();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsKeyPressed(KEY_Q))
            break;

        if (currentState == menuState)
        {
            DrawText("SPACE SHOOTER", screenWidth / 2 - 150, 100, 40, YELLOW);
            DrawText("Select Difficulty:", screenWidth / 2 - 100, 200, 30, YELLOW);
            DrawText("1 - EASY", screenWidth / 2 - 80, 250, 20, YELLOW);
            DrawText("2 - MEDIUM", screenWidth / 2 - 80, 280, 20, YELLOW);
            DrawText("3 - HARD", screenWidth / 2 - 80, 310, 20, YELLOW);
            DrawText("Objective: Destroy enemies & survive.", 250, 360, 20, LIGHTGRAY);
            DrawText("Controls: A D or Left Right Arrow Keys, SPACE Shoot, Q Quit", 155, 390, 20, LIGHTGRAY);
            DrawText("There are 5 levels, at level 3 you get DoubleShot, at level 5 you get TripleShot", 50, 420, 20, LIGHTGRAY);
            DrawText("To WIN you have to clear all five levels ", 250, 450, 20, LIGHTGRAY);
            DrawText("GOOD LUCK ! ", 375, 480, 20, LIGHTGRAY);

            if (IsKeyPressed(KEY_ONE))
                StartGame(0, 1, 2.5f);
            if (IsKeyPressed(KEY_TWO))
                StartGame(1, 2, 3.0f);
            if (IsKeyPressed(KEY_THREE))
                StartGame(2, 3, 3.5);
        }
        else if (currentState == playingState)
        {
            // create stars
            for (int i = 0; i < 100; i++)
            {
                stars[i].y += stars[i].speed; // fall down

                if (stars[i].y > screenHeight)
                {
                    stars[i].x = rand() % screenWidth; // 0 to 899
                    stars[i].y = -10;
                }

                DrawPixel(stars[i].x, stars[i].y, WHITE);
            }

            ship.Update(bullet);
            for (int i = 0; i < activeGerms; i++)
            {
                germs[i].Update();
            }
            for (int i = 0; i < maxBullets; i++)
            {
                bullet[i].Update();
            }

            for (int i = 0; i < maxBullets; i++)
            {
                if (bullet[i].bulletSpeedY <= 0)
                    continue;

                for (int j = 0; j < activeGerms; j++)
                {
                    if (CheckCollisionCircleRec(
                            Vector2{(float)bullet[i].xBullet, (float)bullet[i].yBullet},
                            (float)bullet[i].radius,
                            Rectangle{(float)germs[j].xEnemy, (float)germs[j].yEnemy, (float)germs[j].widthEnemy, (float)germs[j].heightEnemy}))
                    {
                        PlaySound(explosionSound);
                        bullet[i].bulletSpeedY = 0;
                        bullet[i].yBullet = -20;

                        germs[j].yEnemy = -100;
                        germs[j].xEnemy = rand() % (GetScreenWidth() - 20); // 0 to 879

                        playerScore += 10;
                    }
                }
            }
            for (int j = 0; j < activeGerms; j++)
            {
                if (CheckCollisionRecs(
                        Rectangle{(float)ship.xShip, (float)ship.yShip, (float)ship.widthShip, (float)ship.heightShip},
                        Rectangle{(float)germs[j].xEnemy, (float)germs[j].yEnemy, (float)germs[j].widthEnemy, (float)germs[j].heightEnemy}))
                {
                    playerLives -= 2;       // if germ collides with the ship -2 lives
                    germs[j].yEnemy = -100; // remove from screen
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
                    int countIncrease = currentLevel / 2; // increases germs for every even level 2,4
                    float newSpeed = baseEnemySpeed * speedMultiplier;

                    activeGerms = activeGerms + countIncrease;
                    if (activeGerms > maxGerms)
                        activeGerms = maxGerms;
                    if (newSpeed > 8.0)
                        newSpeed = 8.0;

                    // Re-initialize enemies for the new level
                    setEnemies(germs, activeGerms, newSpeed, enemyTexture);
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
            for (int i = 0; i < activeGerms; i++)
            {
                germs[i].Draw();
            }
            for (int i = 0; i < maxBullets; i++)
            {
                bullet[i].Draw();
            }
            // Update high score if playerScore exceeds it
            if (playerScore > highScore[gameMode])
            {
                highScore[gameMode] = playerScore;
                SaveHighScore();
            }

            // Draw high score
            DrawText(TextFormat("Score : %d", playerScore), 10, 10, 20, YELLOW); //%d is used for integars
            DrawText(TextFormat("Lives : %d", playerLives), 10, 40, 20, YELLOW);
            DrawText(TextFormat("Level : %d", currentLevel), 10, 70, 20, YELLOW);
            DrawText(TextFormat("High Score : %d", highScore[gameMode]), 10, 100, 20, GREEN);
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
                DrawText(TextFormat("LEVEL %d", messageLevel), screenWidth / 2 - 60, screenHeight / 2 - 80, 30, Fade(BLUE, messageAlpha));
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
            DrawText("GAME OVER !", screenWidth / 2 - 100, screenHeight / 2 - 50, 40, RED);
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

    UnloadTexture(ship.texture);
    UnloadTexture(bulletTexture);
    UnloadTexture(enemyTexture);
    UnloadSound(explosionSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}