#include "game_logic.h"
#include "object.h"
#include "resetGame.h"
#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <sstream>
#include <random>
#include <mmsystem.h>
#include <vector>
#include <functional>
#include <iomanip>

#define LEVEL_LENGTH 70
#define SCREEN_WIDTH 716
#define JUMP_HEIGHT 120
HBITMAP hbmGameBackground,
        hbmGameBackgroundBlack,hbmGameBackgroundBehind, hbmPlayer, hbmPlayerBlack,hbmPlayerReverse, hbmPlayerBlackReverse,
        hbmSky, hbmCloud1, hbmCloud1Black, hbmBlock,
        hbmBlockBlack, hbmBlockSnow, hbmBlockSnowBlack, hbmTree, hbmTreeBlack,
        hbmRedCandy, hbmRedCandyBlack, hbmBlueCandy, hbmBlueCandyBlack, hbmYellowCandy, hbmYellowCandyBlack,
        hbmPurpleCandy, hbmPurpleCandyBlack, hbmHeart, hbmHeartBlack, hbmEnemy, hbmEnemyBlack, hbmGameOver, hbmCheckpoint,hbmCheckpointBlack,
        hbmBlockForest, hbmBlockInnerForest, hbmTreeForest, hbmTreeForestBlack, hbmFlowers, hbmFlowersBlack, hbmNightBackground,
        hbmMountainBehindDark, hbmBlockNight, hbmInnerBlockNight, hbmTreeNight, hbmTreeNightBlack, hbmBullet, hbmBulletBlack,  hbmCastle, hbmCastleBlack, hbmWin;


BITMAP gameBackgroundBitmap, gameBackgroundBitmapBlack,
       gameBackgroundBehindBitmap,
       skyBitmap, cloud1Bitmap, cloud1BitmapBlack,
       blockBitmap, blockBitmapBlack,
       blockSnowBitmap, blockBitmapSnowBlack,
       treeBitmap, treeBitmapBlack, playerBitmap, playerBitmapBlack, playerBitmapReverse, playerBitmapBlackReverse,
       redCandyBitmap, redCandyBitmapBlack, blueCandyBitmap, blueCandyBitmapBlack, yellowCandyBitmap, yellowCandyBitmapBlack,
       purpleCandyBitmap, purpleCandyBitmapBlack, heartBitmap, heartBitmapBlack, enemyBitmap, enemyBitmapBlack, gameOverBitmap, checkPointBitmap, checkPointBitmapBlack,
       blockForestBitmap,blockInnerForestBitmap, treeForestBitmap,treeForestBitmapBlack, flowersBitmap, flowersBitmapBlack, nightBackgroundBitmap,
       mountainBehindDarkBitmap, blockNightBitmap, innerBlockNightBitmap, treeNightBitmap, treeNightBitmapBlack, castleBitmap,castleBlackBitmap,  bulletBitmap, bulletBitmapBlack,
       winBitmap;


Object gameBackground, gameBackgroundBehind,
       sky, cloud, block, blockSnow, tree, player,candy, enemy, game_over, checkPoint, treeForest, flowers,
       night, bullet, castle;

std::vector<ObjectInfo> terrainBlocks;
std::vector<ObjectInfo> clouds;
std::vector<ObjectInfo> enemies;
std::string timeStr="";
int counter = 0;
int livesCounter = 3;
int candiesCounter = 0;
bool isJumping = false;
int currentLevel = 3;
bool inCastle = false;




void showBackground(HDC hdc, RECT *prect)
{
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prect->right, prect->bottom);
    HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer,hbmBuffer);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld;

    //sky
    if( currentLevel == 3)
    {
        hbmOld = (HBITMAP)SelectObject(hdcMem, hbmNightBackground);
        BitBlt(hdcBuffer, sky.x, sky.y, sky.width, sky.height, hdcMem, 0, 0, SRCCOPY);

    }
    else
    {
        hbmOld = (HBITMAP)SelectObject(hdcMem, hbmSky);
        BitBlt(hdcBuffer, sky.x, sky.y, sky.width, sky.height, hdcMem, 0, 0, SRCCOPY);
    }
    //mountains behind
    if ( currentLevel == 3)
    {
        (HBITMAP)SelectObject(hdcMem, hbmMountainBehindDark);
        BitBlt(hdcBuffer, gameBackgroundBehind.x, gameBackgroundBehind.y, gameBackground.width, gameBackground.height, hdcMem, 0, 0, SRCCOPY);
    }
    else
    {
        (HBITMAP)SelectObject(hdcMem, hbmGameBackgroundBehind);
        BitBlt(hdcBuffer, gameBackgroundBehind.x, gameBackgroundBehind.y, gameBackground.width, gameBackground.height, hdcMem, 0, 0, SRCCOPY);
    }
    //mountain front
    (HBITMAP)SelectObject(hdcMem, hbmGameBackgroundBlack);
    BitBlt(hdcBuffer, gameBackground.x, gameBackground.y, gameBackground.width,gameBackground.height, hdcMem, 0, 0, SRCPAINT);
    (HBITMAP)SelectObject(hdcMem, hbmGameBackground);
    BitBlt(hdcBuffer, gameBackground.x, gameBackground.y, gameBackground.width,gameBackground.height, hdcMem, 0, 0, SRCAND);
    //clouds
    drawClouds(hdcMem, hdcBuffer);
    //enemies
    drawEnemies(hdcMem,hdcBuffer);
    //blocks
    drawLevelTerrain(hdcMem, hdcBuffer);
    //player
    drawPlayer(hdcMem, hdcBuffer);
    //drawHearts
    drawHearts(hdcMem, hdcBuffer);
    //drawBullet
    drawBullet(hdcMem,hdcBuffer, *prect);
    SetBkMode(hdcBuffer, TRANSPARENT);
    TextOut(hdcBuffer, 650, 15, timeStr.c_str(), timeStr.length());
    SetBkMode(hdcBuffer, TRANSPARENT);
    SetTextColor(hdc,RGB(255,0,0));
    TextOut(hdcBuffer, 35, 15, std::to_string(candiesCounter).c_str(), std::to_string(candiesCounter).length());
    //game over
    if(livesCounter <= 0)
    {
        (HBITMAP)SelectObject(hdcMem, hbmGameOver);
        BitBlt(hdcBuffer, 0, 0, gameOverBitmap.bmWidth, gameOverBitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
    }
    if (inCastle == true) {
        (HBITMAP)SelectObject(hdcMem, hbmWin);
        BitBlt(hdcBuffer, 0, 0, winBitmap.bmWidth, winBitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
    }

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);

    BitBlt(hdc, 0, 0, prect->right, prect->bottom, hdcBuffer, 0, 0, SRCCOPY);

    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);
}
void loadGameBitmaps(RECT clientRect)
{
    //background
    hbmGameBackground = (HBITMAP)LoadImage(NULL, "sprites/background_mountain/mountain.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmGameBackground, sizeof(BITMAP), &gameBackgroundBitmap);
    hbmGameBackgroundBlack = (HBITMAP)LoadImage(NULL, "sprites/background_mountain/mountainBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmGameBackgroundBlack, sizeof(BITMAP), &gameBackgroundBitmapBlack);
    gameBackground.width=gameBackgroundBitmap.bmWidth;
    gameBackground.height=gameBackgroundBitmap.bmHeight;
    gameBackground.x=0;
    gameBackground.y=clientRect.bottom -  gameBackgroundBitmap.bmHeight -50;
    //backgorund behind
    hbmGameBackgroundBehind = (HBITMAP)LoadImage(NULL, "sprites/background_mountain/mountainBehind.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmGameBackground, sizeof(BITMAP), &gameBackgroundBehindBitmap);
    gameBackgroundBehind.x=0;
    gameBackgroundBehind.y=clientRect.bottom -  gameBackgroundBitmap.bmHeight -150;
    //sky
    hbmSky = (HBITMAP)LoadImage(NULL, "sprites/background_mountain/sky.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmSky, sizeof(BITMAP), &skyBitmap);
    sky.width = skyBitmap.bmWidth;
    sky.height = skyBitmap.bmHeight;
    //clouds
    hbmCloud1 = (HBITMAP)LoadImage(NULL, "sprites/background_mountain/cloud1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmCloud1, sizeof(BITMAP), &cloud1Bitmap);
    cloud.width = cloud1Bitmap.bmWidth;
    cloud.height = cloud1Bitmap.bmHeight;
    cloud.x = 0;
    cloud.y = 30;
    hbmCloud1Black = (HBITMAP)LoadImage(NULL, "sprites/background_mountain/cloud1Black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmCloud1Black, sizeof(BITMAP), &cloud1BitmapBlack);
    cloud.width = cloud1Bitmap.bmWidth;
    cloud.height = cloud1Bitmap.bmHeight;
    // snow block
    hbmBlockSnow = (HBITMAP)LoadImage(NULL, "sprites/mountain_terrain/tiles_mountain.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmBlockSnow, sizeof(BITMAP), &blockSnowBitmap);
    //snow block black
    hbmBlockSnowBlack = (HBITMAP)LoadImage(NULL, "sprites/mountain_terrain/tiles_mountainBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmBlockSnowBlack, sizeof(BITMAP), &blockBitmapSnowBlack);
    blockSnow.width = blockSnowBitmap.bmWidth;
    blockSnow.height = blockSnowBitmap.bmHeight;
    //no snow block
    hbmBlock = (HBITMAP)LoadImage(NULL, "sprites/mountain_terrain/tiles_mountain_inner.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmBlock, sizeof(BITMAP), &blockBitmap);
    block.width = blockBitmap.bmWidth;
    block.height = blockBitmap.bmHeight;
    //tree
    hbmTree = (HBITMAP)LoadImage(NULL, "sprites/mountain_terrain/mountain_tree.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmTree, sizeof(BITMAP), &treeBitmap);
    //tree
    hbmTreeBlack = (HBITMAP)LoadImage(NULL, "sprites/mountain_terrain/mountain_treeBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmTreeBlack, sizeof(BITMAP), &treeBitmapBlack);
    tree.width = treeBitmap.bmWidth;
    tree.height = treeBitmap.bmHeight;
    //player
    hbmPlayer = (HBITMAP)LoadImage(NULL, "sprites/player/playerSheet.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmPlayer, sizeof(BITMAP), &playerBitmap);
    //player black
    hbmPlayerBlack = (HBITMAP)LoadImage(NULL, "sprites/player/playerSheetBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmPlayerBlack, sizeof(BITMAP), &playerBitmapBlack);
    hbmPlayerReverse = (HBITMAP)LoadImage(NULL, "sprites/player/playerSheetReverse.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmPlayerReverse, sizeof(BITMAP), &playerBitmapReverse);
    //player black
    hbmPlayerBlackReverse = (HBITMAP)LoadImage(NULL, "sprites/player/playerSheetBlackReverse.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmPlayerBlackReverse, sizeof(BITMAP), &playerBitmapBlackReverse);
    player.width = playerBitmap.bmWidth/8;
    player.height = playerBitmap.bmHeight;
    player.x = blockSnow.width;
    player.y = clientRect.bottom - block.height - player.height;
    player.dx = 1;

    //red candy
    hbmRedCandy = (HBITMAP)LoadImage(NULL, "sprites/candies/red_candy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmRedCandy, sizeof(BITMAP), &redCandyBitmap);
    //red candy black
    hbmRedCandyBlack = (HBITMAP)LoadImage(NULL, "sprites/candies/red_candyBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmRedCandyBlack, sizeof(BITMAP), &redCandyBitmapBlack);
    candy.width = redCandyBitmap.bmWidth;
    candy.height = redCandyBitmap.bmHeight;
    //blue candy
    hbmBlueCandy = (HBITMAP)LoadImage(NULL, "sprites/candies/blue_candy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmBlueCandy, sizeof(BITMAP), &blueCandyBitmap);
    //blue candy black
    hbmBlueCandyBlack = (HBITMAP)LoadImage(NULL, "sprites/candies/blue_candyBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmBlueCandyBlack, sizeof(BITMAP), &blueCandyBitmapBlack);
    candy.width = blueCandyBitmap.bmWidth;
    candy.height = blueCandyBitmap.bmHeight;
    //yellow candy
    hbmYellowCandy = (HBITMAP)LoadImage(NULL, "sprites/candies/yellow_candy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmYellowCandy, sizeof(BITMAP), &yellowCandyBitmap);
    //yellow candy black
    hbmYellowCandyBlack = (HBITMAP)LoadImage(NULL, "sprites/candies/yellow_candyBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmYellowCandyBlack, sizeof(BITMAP), &yellowCandyBitmapBlack);
    candy.width = yellowCandyBitmap.bmWidth;
    candy.height = yellowCandyBitmap.bmHeight;
    //purple candy
    hbmPurpleCandy = (HBITMAP)LoadImage(NULL, "sprites/candies/purple_candy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmPurpleCandy, sizeof(BITMAP), &purpleCandyBitmap);
    //purple candy black
    hbmPurpleCandyBlack = (HBITMAP)LoadImage(NULL, "sprites/candies/purple_candyBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmPurpleCandyBlack, sizeof(BITMAP), &purpleCandyBitmapBlack);
    candy.width = purpleCandyBitmap.bmWidth;
    candy.height = purpleCandyBitmap.bmHeight;
    //lives
    hbmHeart = (HBITMAP)LoadImage(NULL, "sprites/lives/heart.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmHeart, sizeof(BITMAP), &heartBitmap);
    //lives black
    hbmHeartBlack = (HBITMAP)LoadImage(NULL, "sprites/lives/heartBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmHeartBlack, sizeof(BITMAP), &heartBitmapBlack);
    candy.width = purpleCandyBitmap.bmWidth;
    candy.height = purpleCandyBitmap.bmHeight;

    //enemy
    hbmEnemy = (HBITMAP)LoadImage(NULL, "sprites/enemy/forest_crawler.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmEnemy, sizeof(BITMAP), &enemyBitmap);
    //enemy black
    hbmEnemyBlack = (HBITMAP)LoadImage(NULL, "sprites/enemy/forest_crawlerBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmEnemyBlack, sizeof(BITMAP), &enemyBitmapBlack);
    enemy.width = enemyBitmap.bmWidth/8;
    enemy.height = enemyBitmap.bmHeight;
    enemy.x = 250;
    enemy.y = clientRect.bottom - blockSnow.height ;

    //game over
    hbmGameOver = (HBITMAP)LoadImage(NULL, "sprites/game_over/gameOver.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmGameOver, sizeof(BITMAP), &gameOverBitmap);

    //win
    hbmWin = (HBITMAP)LoadImage(NULL, "sprites/win/win.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmWin, sizeof(BITMAP), &winBitmap);

    //checkpoint
    hbmCheckpoint = (HBITMAP)LoadImage(NULL, "sprites/checkpoint/checkpoint_sprite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmCheckpoint, sizeof(BITMAP), &checkPointBitmap);
    //checkpoint black
    hbmCheckpointBlack = (HBITMAP)LoadImage(NULL, "sprites/checkpoint/checkpoint_spriteBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmCheckpointBlack, sizeof(BITMAP), &checkPointBitmapBlack);
    checkPoint.width = checkPointBitmap.bmWidth;
    checkPoint.height = checkPointBitmap.bmHeight;
    //forest bitmaps
    //block
    hbmBlockForest = (HBITMAP)LoadImage(NULL, "sprites/forest_terrain/tiles_forest.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmBlockForest, sizeof(BITMAP), &blockForestBitmap);
    //inner block
    hbmBlockInnerForest = (HBITMAP)LoadImage(NULL, "sprites/forest_terrain/tiles_forest_inner.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmBlockInnerForest, sizeof(BITMAP), &blockInnerForestBitmap);
    //tree
    hbmTreeForest = (HBITMAP)LoadImage(NULL, "sprites/forest_terrain/forest_tree.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmTreeForest, sizeof(BITMAP), &treeForestBitmap);

    //tree black
    hbmTreeForestBlack = (HBITMAP)LoadImage(NULL, "sprites/forest_terrain/forest_treeBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmTreeForestBlack, sizeof(BITMAP), &treeForestBitmapBlack);
    treeForest.width = treeForestBitmap.bmWidth;
    treeForest.height = treeForestBitmap.bmHeight; //don't forget to set dimensions of your objects
    //flowers
    hbmFlowers = (HBITMAP)LoadImage(NULL, "sprites/forest_terrain/flowers.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmFlowers, sizeof(BITMAP), &flowersBitmap);

    //tree black
    hbmFlowersBlack = (HBITMAP)LoadImage(NULL, "sprites/forest_terrain/flowersBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmFlowersBlack, sizeof(BITMAP), &flowersBitmapBlack);

    flowers.width = flowersBitmap.bmWidth/4;
    flowers.height = flowersBitmap.bmHeight; //don't forget to set dimensions of your objects

    //night background
    hbmMountainBehindDark = (HBITMAP)LoadImage(NULL, "sprites/castle/mountainBehindDark.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmMountainBehindDark, sizeof(BITMAP), &mountainBehindDarkBitmap);

    hbmNightBackground = (HBITMAP)LoadImage(NULL, "sprites/castle/background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmNightBackground, sizeof(BITMAP), &nightBackgroundBitmap);
    night.width = nightBackgroundBitmap.bmWidth;
    night.height = nightBackgroundBitmap.bmHeight;

    //castle block
    hbmBlockNight = (HBITMAP)LoadImage(NULL, "sprites/castle/tiles_castle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmBlockNight, sizeof(BITMAP), &blockNightBitmap);
    //inner block
    hbmInnerBlockNight = (HBITMAP)LoadImage(NULL, "sprites/castle/tiles_castle_inner.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmInnerBlockNight, sizeof(BITMAP), &innerBlockNightBitmap);

    //tree
    hbmTreeNight = (HBITMAP)LoadImage(NULL, "sprites/castle/castle_tree.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmTreeNight, sizeof(BITMAP), &treeNightBitmap);

    //tree black
    hbmTreeNightBlack = (HBITMAP)LoadImage(NULL, "sprites/castle/castle_treeBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmTreeNightBlack, sizeof(BITMAP), &treeNightBitmapBlack);

    //bullet
    hbmBullet = (HBITMAP)LoadImage(NULL, "sprites/player/ship_bullet.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmBullet, sizeof(BITMAP), &bulletBitmap);

    //tree black
    hbmBulletBlack = (HBITMAP)LoadImage(NULL, "sprites/player/ship_bulletBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmBulletBlack, sizeof(BITMAP), &bulletBitmapBlack);
    bullet.width = bulletBitmap.bmWidth;
    bullet.height = bulletBitmap.bmHeight;

    //castle
    hbmCastle = (HBITMAP)LoadImage(NULL, "sprites/castle/castle_outside.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmCastle, sizeof(BITMAP), &castleBitmap);
    //castle black
    hbmCastleBlack = (HBITMAP)LoadImage(NULL, "sprites/castle/castle_outsideBlack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmCastleBlack, sizeof(BITMAP), &castleBlackBitmap);
    castle.width = castleBlackBitmap.bmWidth;
    castle.height = castleBlackBitmap.bmHeight;

    //initialize terrain
    initializeTerrain(clientRect);

    //generate enemies
    generateEnemies(10 + rand() % 5, clientRect);


    //draw initial clouds
    generateClouds();

}
void drawClouds (HDC hdcMem, HDC hdcBuffer)
{
    for (int i = 0; i < clouds.size(); ++i)
    {
        // Draw black part of the cloud
        SelectObject(hdcMem, hbmCloud1Black);
        BitBlt(hdcBuffer, clouds[i].x, clouds[i].y, cloud.width, cloud.height, hdcMem, 0, 0, SRCPAINT);
        // Draw white part of the cloud
        SelectObject(hdcMem, hbmCloud1);
        BitBlt(hdcBuffer, clouds[i].x, clouds[i].y, cloud.width, cloud.height, hdcMem, 0, 0, SRCAND);
    }
}
void drawPlayer(HDC hdcMem, HDC hdcBuffer)
{
    if(player.direction == RIGHT)
    {
        SelectObject(hdcMem, hbmPlayerBlack);
        BitBlt(hdcBuffer, player.x, player.y, player.width, player.height, hdcMem, player.dx*(player.width), 0, SRCPAINT);

        SelectObject(hdcMem, hbmPlayer);
        BitBlt(hdcBuffer, player.x, player.y, player.width, player.height, hdcMem, player.dx*(player.width), 0, SRCAND);
    }
    else
    {
        SelectObject(hdcMem, hbmPlayerBlackReverse);
        BitBlt(hdcBuffer, player.x, player.y, player.width, player.height, hdcMem, player.dx*(player.width), 0, SRCPAINT);

        SelectObject(hdcMem, hbmPlayerReverse);
        BitBlt(hdcBuffer, player.x, player.y, player.width, player.height, hdcMem, player.dx*(player.width), 0, SRCAND);
    }

}

void drawBullet(HDC hdcMem, HDC hdcBuffer, RECT clientRect)
{
    if(bullet.isVisible)
    {
        SelectObject(hdcMem, hbmBulletBlack);
        BitBlt(hdcBuffer, bullet.x, bullet.y, bullet.width, bullet.height, hdcMem, 0, 0, SRCPAINT);

        SelectObject(hdcMem, hbmBullet);
        BitBlt(hdcBuffer, bullet.x, bullet.y, bullet.width, bullet.height, hdcMem, 0, 0, SRCAND);
        bullet.x += 20;
    }if(bullet.x > clientRect.right){
        bullet.isVisible = false;
        bullet.isSolid = false;
    }

}
void drawEnemies(HDC hdcMem, HDC hdcBuffer)
{
    for (int i = 0; i < enemies.size(); ++i)
    {
        if(enemies[i].isVisible){
        SelectObject(hdcMem, hbmEnemyBlack);
        BitBlt(hdcBuffer, enemies[i].x, enemies[i].y, enemy.width, enemy.height, hdcMem, enemy.dx*(enemy.width), 0, SRCPAINT);

        SelectObject(hdcMem, hbmEnemy);
        BitBlt(hdcBuffer, enemies[i].x, enemies[i].y, enemy.width, enemy.height, hdcMem, enemy.dx*(enemy.width), 0, SRCAND);
        }

    }
}
void generateClouds()
{
    const int cloudCount = 6;
    const int cloudOffsetX = 200;
    const int cloudOffsetY = 30;
    if (clouds.empty())
    {
        for (int i = 0; i < cloudCount; ++i)
        {
            int offsetX = i * cloudOffsetX;
            ObjectInfo newCloud;
            newCloud.x = cloud.x + offsetX;
            newCloud.y = i % 2 == 1 ? cloud.y : cloud.y - cloudOffsetY;
            newCloud.width = cloud.width;
            newCloud.height = cloud.height;
            clouds.push_back(newCloud);
        }
    }
}
void updateClouds(int playerSpeed)
{
    const int cloudOffsetX = 200;
    const int cloudOffsetY = 30;
    for (int i = 0; i < clouds.size(); ++i)
    {
        clouds[i].x -= playerSpeed;
    }
    // Check if the first block is completely off-screen
    if (!clouds.empty() && clouds.front().x + cloud.width <= 0)
    {
        clouds.erase(clouds.begin());
        ObjectInfo newCloud;
        newCloud.x = clouds.back().x + cloudOffsetX;
        newCloud.y = ( rand()% 2 )% 2 == 0 ? cloud.y : cloud.y - cloudOffsetY;
        newCloud.width = cloud.width;
        newCloud.height = cloud.height;
        clouds.push_back(newCloud);

    }
}

void drawHearts(HDC hdcMem, HDC hdcBuffer)
{
    for (int i = 1; i <= livesCounter; ++i)
    {
        SelectObject(hdcMem, hbmHeartBlack);
        BitBlt(hdcBuffer, 270 + i * (candy.width + 10), 10, candy.width, candy.height, hdcMem, 0, 0, SRCPAINT);
        SelectObject(hdcMem, hbmHeart);
        BitBlt(hdcBuffer, 270 + i * (candy.width + 10), 10, candy.width, candy.height, hdcMem, 0, 0, SRCAND);
    }
}
void drawLevelTerrain (HDC hdcMem, HDC hdcBuffer)
{
    SelectObject(hdcMem, hbmRedCandyBlack);
    BitBlt(hdcBuffer, 6,10, candy.width, candy.height, hdcMem, 0, 0, SRCPAINT);
    SelectObject(hdcMem, hbmRedCandy);
    BitBlt(hdcBuffer, 6,10,candy.width, candy.height, hdcMem, 0, 0, SRCAND);
    for (int i = 0; i < terrainBlocks.size(); ++i)
    {
        if(terrainBlocks[i].type == SNOW)
        {
            SelectObject(hdcMem, hbmBlockSnowBlack);
            BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, blockSnow.width, blockSnow.height, hdcMem, 0, 0, SRCPAINT);

            SelectObject(hdcMem, hbmBlockSnow);
            BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y, blockSnow.width, blockSnow.height, hdcMem, 0, 0, SRCAND);

        }
        else if (terrainBlocks[i].type == INNER_BLOCK)
        {
            SelectObject(hdcMem, hbmBlock);
            BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, block.width, block.height, hdcMem, 0, 0, SRCCOPY);


        }
        else if (terrainBlocks[i].type == CASTLE_INNER_BLOCK)
        {
            SelectObject(hdcMem, hbmInnerBlockNight);
            BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, block.width, block.height, hdcMem, 0, 0, SRCCOPY);

        }
        else if (terrainBlocks[i].type == CASTLE)
        {
            SelectObject(hdcMem, hbmBlockNight);
            BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, block.width, block.height, hdcMem, 0, 0, SRCCOPY);

        }
        else if (terrainBlocks[i].type == FOREST)
        {
            SelectObject(hdcMem, hbmBlockForest);
            BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, block.width, block.height, hdcMem, 0, 0, SRCCOPY);


        }
        else if (terrainBlocks[i].type == FOREST_INNER_BLOCK)
        {
            SelectObject(hdcMem, hbmBlockInnerForest);
            BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, block.width, block.height, hdcMem, 0, 0, SRCCOPY);

        }
        else if (terrainBlocks[i].type == TREE)
        {
            SelectObject(hdcMem, hbmTreeBlack);
            BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, tree.width, tree.height, hdcMem, 0, 0, SRCPAINT);

            SelectObject(hdcMem, hbmTree);
            BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y, tree.width, tree.height, hdcMem, 0, 0, SRCAND);

        }
        else if (terrainBlocks[i].type == FLOWER)
        {
            if(terrainBlocks[i].colorIndex == 0)
            {
                SelectObject(hdcMem, hbmFlowersBlack);
                BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, flowers.width, flowers.height, hdcMem, flowers.width, 0, SRCPAINT);

                SelectObject(hdcMem, hbmFlowers);
                BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y, flowers.width, flowers.height, hdcMem, flowers.width, 0, SRCAND);
            }
            else if (terrainBlocks[i].colorIndex == 1)
            {
                SelectObject(hdcMem, hbmFlowersBlack);
                BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, flowers.width, flowers.height, hdcMem, 2*flowers.width, 0, SRCPAINT);

                SelectObject(hdcMem, hbmFlowers);
                BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y, flowers.width, flowers.height, hdcMem, 2*flowers.width, 0, SRCAND);
            }
            else if (terrainBlocks[i].colorIndex == 2)
            {
                SelectObject(hdcMem, hbmFlowersBlack);
                BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, flowers.width, flowers.height, hdcMem, 3*flowers.width, 0, SRCPAINT);

                SelectObject(hdcMem, hbmFlowers);
                BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y, flowers.width, flowers.height, hdcMem, 3*flowers.width, 0, SRCAND);
            }
            else
            {
                SelectObject(hdcMem, hbmFlowersBlack);
                BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, flowers.width, flowers.height, hdcMem, 4*flowers.width, 0, SRCPAINT);

                SelectObject(hdcMem, hbmFlowers);
                BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y, flowers.width, flowers.height, hdcMem, 4*flowers.width, 0, SRCAND);

            }
        }
        else if (terrainBlocks[i].type == FOREST_TREE)
        {
            SelectObject(hdcMem, hbmTreeForestBlack);
            BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, treeForest.width, treeForest.height, hdcMem, 0, 0, SRCPAINT);

            SelectObject(hdcMem, hbmTreeForest);
            BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y, treeForest.width, treeForest.height, hdcMem, 0, 0, SRCAND);
        } else if (terrainBlocks[i].type == CASTLE_TREE) {

            SelectObject(hdcMem, hbmTreeNightBlack);
            BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, treeForest.width, treeForest.height, hdcMem, 0, 0, SRCPAINT);
            SelectObject(hdcMem, hbmTreeNight);
            BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y, treeForest.width, treeForest.height, hdcMem, 0, 0, SRCAND);

        }
        else if (terrainBlocks[i].type == CHECKPOINT)
        {
            SelectObject(hdcMem, hbmCheckpointBlack);
            BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, checkPoint.width,  checkPoint.height, hdcMem, 0, 0, SRCPAINT);

            SelectObject(hdcMem, hbmCheckpoint);
            BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y,  checkPoint.width,  checkPoint.height, hdcMem, 0, 0, SRCAND);

        }
        else if(terrainBlocks[i].type == CANDY && terrainBlocks[i].isVisible == true)
        {
            if(terrainBlocks[i].colorIndex == 0)
            {
                //red
                SelectObject(hdcMem, hbmRedCandyBlack);
                BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, candy.width, candy.height, hdcMem, 0, 0, SRCPAINT);
                SelectObject(hdcMem, hbmRedCandy);
                BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y, candy.width, candy.height, hdcMem, 0, 0, SRCAND);
            }
            else if ( terrainBlocks[i].colorIndex == 1)
            {
                //blue
                SelectObject(hdcMem, hbmBlueCandyBlack);
                BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, candy.width, candy.height, hdcMem, 0, 0, SRCPAINT);
                SelectObject(hdcMem, hbmBlueCandy);
                BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y, candy.width, candy.height, hdcMem, 0, 0, SRCAND);
            }
            else if ( terrainBlocks[i].colorIndex == 2)
            {
                //yellow
                SelectObject(hdcMem, hbmYellowCandyBlack);
                BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, candy.width, candy.height, hdcMem, 0, 0, SRCPAINT);
                SelectObject(hdcMem, hbmYellowCandy);
                BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y, candy.width, candy.height, hdcMem, 0, 0, SRCAND);
            }
            else
            {
                //purple
                SelectObject(hdcMem, hbmPurpleCandyBlack);
                BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, candy.width, candy.height, hdcMem, 0, 0, SRCPAINT);
                SelectObject(hdcMem, hbmPurpleCandy);
                BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y, candy.width, candy.height, hdcMem, 0, 0, SRCAND);

            }
        }
          else if(terrainBlocks[i].type == CASTLE_CHECKPOINT) {
            SelectObject(hdcMem, hbmCastleBlack);
            BitBlt(hdcBuffer, terrainBlocks[i].x, terrainBlocks[i].y, castle.width,  castle.height, hdcMem, 0, 0, SRCPAINT);
            SelectObject(hdcMem, hbmCastle);
            BitBlt(hdcBuffer, terrainBlocks[i].x,terrainBlocks[i].y,  castle.width,  castle.height, hdcMem, 0, 0, SRCAND);
        }
    }
}
void initializeTerrain(RECT clientRect)
{
    std::srand(static_cast<unsigned> (time(nullptr)));
    int totalCreatedBlocks = 0;
    int terrainType = SNOW;
    int innerTerrainType = INNER_BLOCK;
    int treeType = TREE;

    if(currentLevel == 1)
    {
        terrainType = SNOW;
        treeType = TREE;
        innerTerrainType = INNER_BLOCK;
    }
    else if(currentLevel == 2)
    {
        terrainType = FOREST;
        innerTerrainType = FOREST_INNER_BLOCK;
        treeType = FOREST_TREE;
    }
    else
    {
        terrainType = CASTLE;
        innerTerrainType = CASTLE_INNER_BLOCK;
        treeType = CASTLE_TREE;
    }
    generateLevelTerrain(3, clientRect, terrainType, treeType);
    while(totalCreatedBlocks <= LEVEL_LENGTH)
    {
        int selectFunction = std::rand() % 10; //0-9
        int levelBlocks =  2+ std::rand() % 3;

        int hillBlocksHeight = 2 + std::rand() % 3;

        int hillBlocksWidth = 4 + std::rand() % 4;

        if(selectFunction <= 5)   //60%level terrain
        {
            generateLevelTerrain(levelBlocks, clientRect, terrainType, treeType);
            totalCreatedBlocks += levelBlocks;
        }
        else
        {
            if(selectFunction > 5 && selectFunction < 9 ) //30%
            {
                generateHillTerrain(hillBlocksWidth, hillBlocksHeight, clientRect,terrainType, innerTerrainType);
                totalCreatedBlocks += hillBlocksWidth;
            }
            else
            {

                //10%
            }

        }
    }
    generateLevelTerrain(3, clientRect, terrainType, treeType);
    generateLevelTerrain(5, clientRect, terrainType, treeType);
      if(currentLevel == 3) {
        generateLevelTerrain(5, clientRect, terrainType, treeType);
        generateCastleCheckpoint(clientRect);


    } else {
        generateCheckpoint(clientRect);
        generateLevelTerrain(5, clientRect, terrainType, treeType);
    }
}
bool isColliding( Object& player,  Object& block)
{
    return (block.x < player.x + player.width  &&
            block.x + block.width > player.x &&
            block.y < player.y + player.height &&
            block.y + block.height > player.y && (block.type == SNOW || block.type == FOREST || block.type == CASTLE));
}
bool isOnCheckPoint( Object& player,  Object& block)
{
    return (block.x < player.x + player.width  &&
            block.x + block.width > player.x &&
            block.y < player.y + player.height &&
            block.y + block.height > player.y && block.type == CHECKPOINT  && block.isSolid);
}

bool isInCastle( Object& player,  Object& block)
{
    return (block.x < player.x &&
            block.x + block.width > player.x &&
            block.type == CASTLE_CHECKPOINT && block.isSolid);
}
bool isCollidingReverse( Object& player,  Object& block)
{
    return (block.x + block.width< player.x + player.width &&
            block.x  > player.x - player.width  &&
            block.y <= player.y + player.height &&
            block.y + block.height >= player.y &&
            (block.type == INNER_BLOCK || block.type == FOREST_INNER_BLOCK || block.type == CASTLE_INNER_BLOCK));
}

bool isInTheAir( Object& player,  Object& block)
{
    return (block.x < player.x + player.width / 2 &&
            block.x + block.width > player.x + player.width / 2 &&
            player.y + player.height < block.y &&
            (block.type == SNOW || block.type == FOREST || block.type == CASTLE));
}
bool isOnGround(Object& player, Object& block)
{
    return ((block.type == SNOW || block.type == FOREST || block.type == CASTLE) &&
            block.x < player.x + player.width &&
            block.x + block.width > player.x &&
            player.y + player.height == block.y && block.isSolid);
}
bool isCollidingWithCandy(const Object& player, const Object& blockTmp)
{

    return (blockTmp.x + blockTmp.width > player.x &&
            blockTmp.x < player.x + player.width  &&
            blockTmp.y + blockTmp.height > player.y  &&
            blockTmp.y < player.y + player.height  &&
            blockTmp.type == CANDY &&
            blockTmp.isVisible == true);

}
void applyGravity()
{
    for( Object& block : terrainBlocks)
    {
        if(isInTheAir(player, block))
        {
            player.y +=10;


        }
        else if (isOnGround(player, block))
        {

            isJumping = false;
        }

    }
}

void playCoinSound(const std::string& alias)
{
    // Open the sound file with the generated alias
    std::string openCommand = "open \"sounds/sound_coin.wav\" type waveaudio alias " + alias;
    mciSendString(openCommand.c_str(), NULL, 0, NULL);

    // Play the sound using the generated alias
    std::string playCommand = "play " + alias;
    mciSendString(playCommand.c_str(), NULL, 0, NULL);
}





void playJumpSound(const std::string& alias)
{
    // Open the jump sound file with the generated alias
    std::string openCommand = "open \"sounds/jump_sound.wav\" type waveaudio alias " + alias;
    mciSendString(openCommand.c_str(), NULL, 0, NULL);

    // Play the sound using the generated alias
    std::string playCommand = "play " + alias;
    mciSendString(playCommand.c_str(), NULL, 0, NULL);
}

// Similar functions can be created for other sounds...

void playSoundAsync(const std::function<void(const std::string&)>& soundFunction)
{
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::stringstream aliasStream;
    aliasStream << "sound_" << timestamp;
    std::string alias = aliasStream.str();

    std::thread soundThread(soundFunction, alias);

    soundThread.detach();
}


void updatePlayer(int action, RECT clientRect)
{
        if(player.dx == 7)
        {
            player.dx = 1;
        }
    static int step = 10;
    int trackingPoint = SCREEN_WIDTH / 2;
    if(action == RIGHT)
    {

        player.dx++;
        player.direction = RIGHT;
        if(isPastTrackingPoint(player.x))
        {
            updateTerrain(step);
            updateClouds(step);
            updateEnemies(step);
            player.x = trackingPoint;
        }
        for( Object& block : terrainBlocks)
        {
            if(isColliding(player,block))
            {
                player.y = block.y - player.height; //penje se

            }
            if(isCollidingWithCandy(player,block))
            {
                block.isVisible = false;
                candiesCounter++;
                playSoundAsync(playCoinSound);
            }
            if(isOnCheckPoint(player, block))
            {
                block.isSolid = false;
                ++currentLevel;
                //if you get undefined reference here
                //add resetGame.h and resetGame.cpp to project files
                resetTerrain(clientRect);
            }

               if(isInCastle(player, block)){
                inCastle = true;
               }
        }
        player.x += step;
    }
    else if(action == LEFT)
    {
       player.dx++;
        player.direction = LEFT;
        for( Object& block : terrainBlocks)
        {
            if(isCollidingReverse(player,block))
            {
                player.x += step; //ne moze nazad

            }
            if(isCollidingWithCandy(player,block))
            {
                block.isVisible = false;
                candiesCounter++;
                playSoundAsync(playCoinSound);

            }
        }

        player.x > 0 ? player.x -= step :  player.x -= 0;


    }
    else if (action == JUMP && !isJumping)
    {
        playSoundAsync(playJumpSound);
        player.y -= JUMP_HEIGHT;
        isJumping = true;
    }
    else if(action == SHOOT)
    {
        PlaySound("sounds/shoot.wav", NULL, SND_FILENAME | SND_ASYNC);
        bullet.x = player.x + player.width/2;
        bullet.y = player.y + player.height/2;
        bullet.isVisible = true;
        bullet.isSolid = true;

    }
    else
    {
        //
    }

}
void updateEnemiesMovement()
{
    const int collisionOffset = 20;
    int shrinkAmount = 15;
    static int enemyStep = 5;
    if(enemy.dx == 7)
    {
        enemy.dx = 1;
    }
    enemy.dx++;
    for (Object& enemy : enemies)
    {
        enemy.x -= enemyStep;
        if( player.x + shrinkAmount < enemy.x + enemy.width - shrinkAmount &&
                player.x + player.width - shrinkAmount > enemy.x + shrinkAmount &&
                player.y + shrinkAmount < enemy.y + enemy.height - shrinkAmount &&
                player.y + player.height - shrinkAmount > enemy.y + shrinkAmount &&
                enemy.isSolid && enemy.isVisible)
        {
            livesCounter--; //disabled for debugging
            enemy.isSolid = false;

        }
        if (bullet.x + collisionOffset < enemy.x + enemy.width &&
            bullet.x + bullet.width - collisionOffset > enemy.x &&
            bullet.y + collisionOffset < enemy.y + enemy.height &&
            bullet.y + bullet.height - collisionOffset > enemy.y &&
            bullet.isVisible && bullet.isSolid && enemy.isVisible){
            enemy.isVisible = false;
            bullet.isVisible = false;
            bullet.isSolid = false;
        }
    }

}

bool isPastTrackingPoint(int x)
{
    return x >= SCREEN_WIDTH/2;
}
void generateLevelTerrain(int length, RECT clientRect, int blockLevel, int treeType)
{
    int startingX =  0;
    ObjectInfo newBlock;
    ObjectInfo newTree;
    Object newFlower;
    if(!terrainBlocks.empty())
        startingX = terrainBlocks.back().x + blockSnow.width;
    for (int i = 0; i <= length; ++i)
    {
        if(i % 7 == 0)
        {
            newTree.type = treeType;
            newTree.isSolid = false;
            newTree.x = startingX + i* blockSnow.width + blockSnow.width/2;
            if(treeType == FOREST_TREE || treeType == CASTLE_TREE)
            {
                newTree.y = clientRect.bottom - blockSnow.height - treeForest.height;
            }
            else
            {
                newTree.y = clientRect.bottom - blockSnow.height - tree.height;
            }
            terrainBlocks.push_back(newTree);
        }
        else if(  currentLevel == 2 || currentLevel ==3 )
        {
            newFlower.type = FLOWER;
            newFlower.isSolid = false;
            newFlower.colorIndex =  rand() % 4;
            newFlower.x = startingX + i* blockSnow.width + blockSnow.width/3;
            newFlower.y = clientRect.bottom - blockSnow.height - flowers.height + 7;
            terrainBlocks.push_back(newFlower);
        }
        newBlock.isSolid = true;
        newBlock.type = blockLevel;
        newBlock.width = blockSnow.width;
        newBlock.height = blockSnow.height;
        newBlock.y = clientRect.bottom - blockSnow.height;
        newBlock.x = startingX + i* blockSnow.width;
        terrainBlocks.push_back(newBlock);

    }

}
void generateCheckpoint(RECT clientRect)
{
    ObjectInfo newBlock;
    newBlock.isSolid = true;
    newBlock.type = CHECKPOINT;
    newBlock.y = clientRect.bottom - blockSnow.height - checkPoint.height + 25;
    newBlock.x = terrainBlocks.back().x - blockSnow.width;
    terrainBlocks.push_back(newBlock);

}

void generateCastleCheckpoint(RECT clientRect) {
    ObjectInfo newBlock;
    newBlock.isSolid = true;
    newBlock.type = CASTLE_CHECKPOINT;
    newBlock.y = clientRect.bottom - blockSnow.height - castle.height + 25;
    newBlock.x = terrainBlocks.back().x - 7*blockSnow.width;
    terrainBlocks.push_back(newBlock);

}
void generateEnemies(int numEnemies, RECT clientRect)
{
    Object newEnemy;
    std::vector<int> snowBlockIndices; //enemy can only spawn on level terrain
    for (int i = 0; i < terrainBlocks.size(); ++i)
    {
        if ((terrainBlocks[i].type == SNOW || terrainBlocks[i].type == FOREST || terrainBlocks[i].type == CASTLE )&& terrainBlocks[i].y == clientRect.bottom - blockSnow.height && terrainBlocks[i].x > player.x + 200)
        {
            snowBlockIndices.push_back(i);
        }
    }

    // If there are no SNOW blocks, return
    if (snowBlockIndices.empty())
    {
        return;
    }
    for (int i = 0; i < numEnemies; ++i)
    {
        newEnemy.type = ENEMY;
        newEnemy.width = enemy.width;
        newEnemy.height = enemy.height;

        // Randomly choose a position on the terrain
        int randomSnowIndex = snowBlockIndices[rand() % snowBlockIndices.size()];
        const Object& snowBlock = terrainBlocks[randomSnowIndex];

        newEnemy.x = snowBlock.x;
        newEnemy.y = snowBlock.y - newEnemy.height + 10;
        newEnemy.isSolid = true;
        enemies.push_back(newEnemy);
    }
    std::cout << "Num of enemies" << enemies.size() << std::endl;

}

void generateHillTerrain(int length, int height, RECT clientRect, int blockLevel, int innerBlockLevel)
{
    int startingX = 0;
    if (!terrainBlocks.empty())
        startingX = terrainBlocks.back().x;
    Object newBlock;
    Object newCandy;
    Object newFlower;

    for (int i = 1; i <= length; ++i)
    {
        for (int j = 1; j <= i && j <= height; ++j)
        {

            newBlock.isSolid = true;
            // Use SNOW for outer blocks and TREE for inner blocks
            newBlock.type = (j == i || j == height) ? blockLevel : innerBlockLevel;
            newBlock.width = blockSnow.width;
            newBlock.height = blockSnow.height;
            newBlock.x = startingX + i * blockSnow.width;
            newBlock.y = clientRect.bottom - j * blockSnow.height;
            terrainBlocks.push_back(newBlock);

            if ((j == i || j == height) &&  rand() % 2 == 0 )
            {
                newCandy.type = CANDY;
                newCandy.colorIndex = rand() % 4;
                newCandy.isSolid = false;
                newCandy.isVisible = true;
                newCandy.x = startingX + i*blockSnow.width;
                newCandy.y = clientRect.bottom - j*blockSnow.height - candy.height*1.5;
                terrainBlocks.push_back(newCandy);
            }

        }
    }
}


void updateTerrain(int playerSpeed)
{
    // Move existing blocks to the left
    for (int i = 0; i < terrainBlocks.size(); ++i)
    {
        terrainBlocks[i].x -= playerSpeed;
    }


    // Check if the first block is completely off-screen
    if (!terrainBlocks.empty() && terrainBlocks.front().x + blockSnow.width <= 0)
    {
        // Remove the first block
        terrainBlocks.erase(terrainBlocks.begin());

    }
}
void updateEnemies(int playerSpeed)
{
    // Move existing blocks to the left
    for (int i = 0; i < enemies.size(); ++i)
    {
        enemies[i].x -= playerSpeed;

    }


    // Check if the first block is completely off-screen
    if (!enemies.empty() && enemies.front().x + enemy.width <= 0)
    {
        // Remove the first block
        enemies.erase(enemies.begin());

    }
}
void updateClock()
{

    counter += 1000;

    int seconds = (counter / 1000) % 60;
    int minutes = (counter / (1000 * 60)) % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;
    timeStr = oss.str();
}
