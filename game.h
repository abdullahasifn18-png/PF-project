#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <fstream>
#include <iostream>
using namespace std;

// extern is similar to pass by reference, only copy made memory isnt allocated
// memory is allocated when it is decalared in .cpp
extern int highScore[3];

extern Texture2D bulletTexture;
extern Texture2D enemyTexture;

void LoadHighScore();
void SaveHighScore();

const int maxBullets = 100;
const int maxGerms = 7; 
const int maxLevel = 5;
const int perLevelScore = 100;
const int screenWidth = 900;
const int screenHeight = 700;

const int desiredShipWidth = 100;
const int desiredShipHeight = 75;
const int desiredBulletSize = 40;
const int desiredEnemySize = 50;

const int menuState = 0;
const int playingState = 1;
const int gameOverState = 2;
const int gameWonState = 3;

extern int currentState;
extern int currentLevel;
extern int gameMode;
extern int activeGerms; 
extern int playerScore;
extern int playerLives;
extern float baseEnemySpeed;

// structs
struct Star {
    int x, y;
    float speed;
};

struct bullets {
    int radius;
    int xBullet, yBullet;
    int bulletSpeedX, bulletSpeedY;
    Texture2D texture;

    void Draw();
    void Update();
};

struct spaceShip {
    float xShip, yShip;
    int widthShip, heightShip;
    int speedShip;
    Texture2D texture;

    void Draw();
    void Update(bullets bullet[]);
};

struct enemy {
    int xEnemy, yEnemy;
    int widthEnemy, heightEnemy;
    float yVelocity, xVelocity;
    
    Texture2D texture;

    void Draw();
    void Update();
};

// definitions
void setEnemies(enemy a[], int maxGerms, float speedValue, Texture2D tex);
void resetGame(int screenWidth, int screenHeight);
void StartGame(int mode, int germsCount, float speed);

#endif