#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include <windows.h>

void showBackground(HDC hdc, RECT *prect);
void loadGameBitmaps(RECT clientRect);
void drawClouds (HDC hdcMem, HDC hdcBuffer);
void drawLevelTerrain(HDC hdcMem, HDC hdcBuffer);
void initializeTerrain(RECT clientRect);
void updateTerrain(int playerSpeed);
void generateLevelTerrain(int , RECT, int, int);
void generateHillTerrain(int , int , RECT , int, int);
void updateClock();
void drawPlayer(HDC hdcMem, HDC hdcBuffer);
void updatePlayer(int action, RECT clientRect);
bool isPastTrackingPoint(int x);
void generateClouds();
void drawBullet(HDC hdcMem, HDC hdcBuffer, RECT clientRect);
void updateClouds(int playerSpeed);
void applyGravity();
void drawCandies(HDC hdcMem, HDC hdcBuffer);
void updateCandies(int playerSpeed);
void drawEnemies(HDC hdcMem, HDC hdcBuffer);
void updateEnemies(int playerSpeed);
void updateEnemiesMovement();
void generateEnemies(int numEnemies, RECT clientRect);
void drawHearts(HDC hdcMem, HDC hdcBuffer);
void generateCheckpoint(RECT clientRect);
void generateCastleCheckpoint(RECT clientRect);


#endif // GAME_LOGIC_H

