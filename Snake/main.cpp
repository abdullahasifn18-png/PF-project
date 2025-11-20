#include <iostream>
#include <raylib.h>
using namespace std;
Color green={173,204,96,255};
Color darkGreen={43,51,24,255};
//creating a grid
int cellSize=30;
int cellCount=25;//for each row and col also 25x30=750 yani 750 by 750 ko cover krega

struct Food{
    Vector2 position={5,6};//NOT PIXELS BUT CELLS, creates food at x and y coords like xth row mein 6th col

    void Draw()
    {
        DrawRectangle(position.x *cellSize,position.y *cellSize,cellSize,cellSize,darkGreen);
    }
};

int main(){
cout<<"Starting the game..."<<endl;
InitWindow(cellSize*cellCount,cellSize*cellCount," Retro Snake");//width, height, window name
SetTargetFPS(60);//how fast the game should run

Food food=Food();//object of a struct
while(WindowShouldClose()==false)
{
    BeginDrawing();

    ClearBackground(green);
    food.Draw();

    EndDrawing();
}


CloseWindow();
}