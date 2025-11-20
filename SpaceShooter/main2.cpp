#include <iostream>
#include <raylib.h>
using namespace std;
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
spaceShip ship;
int main(){
    const int screenWidth = 1280;
    const int screenHeight = 800;
    InitWindow(screenWidth,screenHeight,"Space Shooter");
    SetTargetFPS(60);  
    ship.width = 50;
    ship.height = 25;
    ship.x = screenHeight - ship.height - 10;
    ship.y = screenWidth / 2 - ship.width / 2; // the height of rectangle starts from the top left of the rec
    ship.speed = 8;
    
    //game loop 
    while (WindowShouldClose() == false){
        BeginDrawing();
        ship.Update();
        ship.Draw();

        ClearBackground(BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}