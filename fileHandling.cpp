#include <iostream>
#include <fstream>
using namespace std;
int highScore = 0;
void LoadHighScore()
{
    ifstream file("highscore.txt");

    if (file.is_open())
    {
        file >> highScore;
        file.close();
    }
    else
    {
        highScore = 0; // If file doesn't exist yet
    }
}
void SaveHighScore()
{
    ofstream file("highscore.txt");
    file << highScore;
    file.close();
}
LoadHighScore();
if (playerScore > highScore)
{
    highScore = playerScore;
    SaveHighScore();
}
DrawText(TextFormat("High Score: %d", highScore), 20, 20, 25, GREEN);