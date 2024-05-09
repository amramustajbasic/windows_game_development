#include "intro.h"
#include "object.h"
#include "resetGame.h"
HBITMAP hbmintro, hbmintroTextOne, hbmintroTextOneBlack;
BITMAP introBitmap, introTextOneBitmap,introTextOneBitmapBlack;
Object intro, introTextOne;
Button skipButton;
void loadIntroBitmaps(RECT clientRect)
{
    //intro
    hbmintro = (HBITMAP)LoadImage(NULL, "sprites/intro/intro_sheet.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmintro, sizeof(BITMAP), &introBitmap);
    intro.width=introBitmap.bmWidth;
    intro.height=introBitmap.bmHeight;

    hbmintroTextOne = (HBITMAP)LoadImage(NULL, "sprites/intro/intro_text1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmintroTextOne, sizeof(BITMAP), &introTextOneBitmap);

    hbmintroTextOneBlack = (HBITMAP)LoadImage(NULL, "sprites/intro/intro_text1black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmintroTextOneBlack, sizeof(BITMAP), &introTextOneBitmapBlack);
    introTextOne.width=introBitmap.bmWidth;
    introTextOne.height=introBitmap.bmHeight;
    introTextOne.x = 0;
    introTextOne.y = clientRect.bottom;
}
void DrawIntro(HDC hdc, RECT* prect)
{
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prect->right, prect->bottom);
    HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer,hbmBuffer);

    HDC hdcMem = CreateCompatibleDC(hdc);

    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmintro);
    BitBlt(hdcBuffer, intro.x, intro.y, intro.width, intro.height, hdcMem, 0, 0, SRCCOPY);

    (HBITMAP)SelectObject(hdcMem, hbmintroTextOneBlack);
    BitBlt(hdcBuffer, introTextOne.x, introTextOne.y, introTextOne.width, introTextOne.height, hdcMem, 0, 0, SRCPAINT);
    (HBITMAP)SelectObject(hdcMem, hbmintroTextOne);
    BitBlt(hdcBuffer, introTextOne.x, introTextOne.y, introTextOne.width, introTextOne.height, hdcMem, 0, 0, SRCAND);


    SetBkMode(hdcBuffer,TRANSPARENT);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdcBuffer, GetStockObject(HOLLOW_BRUSH));
    Rectangle(hdcBuffer,skipButton.rect.left,skipButton.rect.top,skipButton.rect.right,skipButton.rect.bottom);
    DrawText(hdcBuffer, "Skip", -1, &skipButton.rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);

    BitBlt(hdc, 0, 0, prect->right, prect->bottom, hdcBuffer, 0, 0, SRCCOPY);

    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);
}
void updateText()
{
    introTextOne.y -=2;
}
void playIntroMusic()
{
    PlaySound("sounds/intro_sound.wav", NULL, SND_FILENAME | SND_ASYNC);
}
void stopIntroMusic()
{
    PlaySound(NULL, NULL, 0);
}
void InitializeSkipButton(RECT clientRect)
{
    // Adjust these values to position the buttons in the bottom-right corner
    int buttonWidth = 80;
    int buttonHeight = 30;
    int buttonMargin = 20;

    // Initialize start button
   skipButton.rect.left = clientRect.right - buttonWidth - buttonMargin;
   skipButton.rect.top = clientRect.bottom - buttonHeight - 20;

   skipButton.rect.right =skipButton.rect.left + buttonWidth;
   skipButton.rect.bottom =skipButton.rect.top + buttonHeight;
   skipButton.type = START_BUTTON;


}
void HandleIntroButtonClick(HWND hwnd, int mouseX, int mouseY, bool *intro, RECT clientRect)
{
    if (PtInRect(&skipButton.rect, { mouseX, mouseY }))
    {
        KillTimer(hwnd, INTRO_TIMER);
        SetTimer(hwnd, GAME_TIMER, 32, NULL );
        stopIntroMusic();
        SetTimer(hwnd, CLOCK_TIMER, 1000, NULL);
        resetGame(hwnd, clientRect);


    }

}
