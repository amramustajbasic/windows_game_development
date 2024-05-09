#ifndef RESET_H
#define RESET_H
#include <windows.h>
#include <vector>
#include <iostream>
#include "object.h"
void resetGame(HWND hwnd, RECT clientRect);
void resetTerrain(RECT clientRect);
extern std::vector<ObjectInfo> terrainBlocks;
extern std::vector<ObjectInfo> enemies;
extern std::string timeStr;
extern int counter;
extern int livesCounter;
extern int candiesCounter;
extern Object player;
extern int currentLevel;
extern bool inCastle;
#endif // RESET_H
