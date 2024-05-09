#include "resetGame.h"
#include "game_logic.h"
void resetTerrain(RECT clientRect)
{
    int numOfEnemies = 12;
    terrainBlocks.clear();
    enemies.clear();
    initializeTerrain(clientRect);
    player.x = 50;
    generateEnemies(numOfEnemies + rand() % 5, clientRect);
}

void resetGame(HWND hwnd, RECT clientRect)
{
    KillTimer(hwnd, GAME_TIMER);
    KillTimer(hwnd, CLOCK_TIMER);
    livesCounter = 3;
    candiesCounter = 0;
    currentLevel = 1;
    inCastle = false;
    timeStr = "";
    counter = 0;
    resetTerrain(clientRect);
    SetTimer(hwnd, GAME_TIMER, 32, NULL);
    SetTimer(hwnd, CLOCK_TIMER, 1000, NULL);

}
