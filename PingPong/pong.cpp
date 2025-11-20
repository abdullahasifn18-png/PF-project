
#include <iostream>
#include <raylib.h>
using namespace std;

int playerScore = 0;
int cpuScore = 0;

struct Ball
{
    float x, y;
    int speedX, speedY;
    int radius;
    void Draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }
    void Update()
    { // to make the ball move
        x += speedX;
        y += speedY; // increases the x and y coords by 7 pixels for gameball as set in object gameball

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {                 // we do +radius for lower edge of ball and -radius for upper edge of ball
            speedY *= -1; // when the lower or upper point of the ball touch the lower or upper edge of window we bounce it in the opposite direction and it starts to update its position by +-7 each time
        }
        if (x + radius >= GetScreenWidth()) // cpu wins
        {
            cpuScore++;
            resetBall();
        }
        if (x - radius <= 0)
        { // player wins
            playerScore++;
            resetBall();
        }
    }
    void resetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speedChoices[2] = {-1, 1};
        speedX *= speedChoices[GetRandomValue(0, 1)];
        speedY *= speedChoices[GetRandomValue(0, 1)];
    }
};
struct Paddle
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
        if (IsKeyDown(KEY_UP))
        {
            y = y - speed; // as neeche positive y
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            y = y + speed; // as neeche positive y
        }
        if (y <= 0)
        { // for not moving out of the upper bound
            y = 0;
        }
         else if (y + height >= GetScreenHeight())
        {
         y = GetScreenHeight() - height;
        
        }
    }
};
struct CpuPaddle
{
    // attribute or qualities
    float x, y;
    float width, height;
    int speed; // only for y axis as only up down
    void Draw()
    { // functions inside structs are called methods
         DrawRectangle(x, y, width, height, WHITE);
    }
    void Update(int ballY)
    {
         if (y + height / 2 > ballY)
        {                          // y+height/2 gives us the center of the paddle
         y = y - speed; // as neeche positive y
        
        }
         else if (y + height / 2 < ballY)
        {
         y = y + speed; // as neeche positive y
        
        }
         if (y <= 0)
        { // for not moving out of the upper bound
         y = 0;
        
        }
        else if (y + height >= GetScreenHeight())
        {
             y = GetScreenHeight() - height;
            
        }
    }
};

Ball gameball;       // an object/instance is created
Paddle playerPaddle; // paddle object named player
CpuPaddle cpu;
int main()
{
    cout << "Starting the Game " << endl;

    //        //creating game window
    const int screenWidth = 1280;
    const int screenHeight = 800;
    //      //int recY=screenHeight/2-60;  //   screenHeight/2-recHeight/2
    InitWindow(screenWidth, screenHeight, "My Pong Game"); // width, height, title
    SetTargetFPS(60);                                      // game speed
                                                           //     //initializations
    //     //now we will interact with gameball's element
    gameball.radius = 20;
    gameball.x = screenWidth / 2;
    gameball.y = screenHeight / 2;
    gameball.speedX = 7;
    gameball.speedY = 7;
    //   //now we will interact with the elements of object player
    playerPaddle.width = 25;
    playerPaddle.height = 120;
    playerPaddle.x = screenWidth - playerPaddle.width - 10;
    playerPaddle.y = screenHeight / 2 - playerPaddle.height / 2; // the height of rectangle starts from the top left of the rec
    playerPaddle.speed = 7;
    //     //interacting with elements of CpuPaddle
    cpu.height = 120;
    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screenHeight / 2 - cpu.height / 2; // the height of rectangle starts from the top left of the rec
    cpu.speed = 6;
    //     //game loop
    while (WindowShouldClose() == false) // it returns true when esc key is pressed or window is closed

    {
        BeginDrawing(); // we call two functions begin and end drawing function
                        //            //Updating
        gameball.Update();
        playerPaddle.Update();
        cpu.Update(gameball.y); // pass the y coord of the ball

        if (CheckCollisionCircleRec(Vector2{gameball.x, gameball.y}, gameball.radius, Rectangle{playerPaddle.x, playerPaddle.y, playerPaddle.width, playerPaddle.height}))
        {                          // for player paddle
                                   //                    //if true we have a collision
            gameball.speedX *= -1; // reverses ball speed
        }
        if (CheckCollisionCircleRec(Vector2{gameball.x, gameball.y}, gameball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            //                    //if true we have a collision
            gameball.speedX *= -1; // reverses ball speed
        }

        //            //Drawing
        ClearBackground(BLACK);
        DrawLine(screenWidth / 2, 0, screenWidth / 2, 800, WHITE);
        gameball.Draw(); // function called
        cpu.Draw();
        //            //DrawRectangle(10,recY,25,120,WHITE);
        playerPaddle.Draw();
        DrawText(TextFormat("%i", cpuScore), screenWidth / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", playerScore), 3 * screenWidth / 4 - 20, 20, 80, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}