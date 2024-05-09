#include "mainMenu.h"
#include "intro.h"
Button startButton, exitButton;
HBITMAP hbmBackground;
BITMAP backgroundBitmap;
Object background;
void DrawMainMenu(HDC hdc, RECT* prect)
{
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prect->right, prect->bottom);
    HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer,hbmBuffer);

    HDC hdcMem = CreateCompatibleDC(hdc);

    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmBackground);
    BitBlt(hdcBuffer, background.x, background.y, background.width, background.height, hdcMem, 0, 0, SRCCOPY);


    SetBkMode(hdcBuffer,TRANSPARENT);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdcBuffer, GetStockObject(HOLLOW_BRUSH));

    // Draw start button
    Rectangle(hdcBuffer, startButton.rect.left, startButton.rect.top, startButton.rect.right, startButton.rect.bottom);
    DrawText(hdcBuffer, "Start Game", -1, &startButton.rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Draw exit button
    Rectangle(hdcBuffer, exitButton.rect.left, exitButton.rect.top, exitButton.rect.right, exitButton.rect.bottom);
    DrawText(hdcBuffer, "Exit", -1, &exitButton.rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);

    BitBlt(hdc, 0, 0, prect->right, prect->bottom, hdcBuffer, 0, 0, SRCCOPY);

    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);
}
void InitializeButtons(RECT clientRect)
{
    // Adjust these values to position the buttons in the bottom-right corner
    int buttonWidth = 280;
    int buttonHeight = 40;
    int buttonMargin = 20;

    // Initialize start button
    startButton.rect.left = clientRect.right - buttonWidth - buttonMargin - 40;
    startButton.rect.top = clientRect.bottom - buttonHeight - 150;

    startButton.rect.right = startButton.rect.left + buttonWidth;
    startButton.rect.bottom = startButton.rect.top + buttonHeight;
    startButton.type = START_BUTTON;

    // Initialize exit button
    exitButton.rect.left = clientRect.right - buttonWidth - buttonMargin - 40;
    exitButton.rect.top = startButton.rect.bottom + buttonMargin ;

    exitButton.rect.right = exitButton.rect.left + buttonWidth;
    exitButton.rect.bottom = exitButton.rect.top + buttonHeight;
    exitButton.type = EXIT_BUTTON;
}
void loadMainMenuBitmaps(RECT clientRect)
{
    //background
    hbmBackground = (HBITMAP)LoadImage(NULL, "sprites/mainMenu/mainMenuBackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmBackground, sizeof(BITMAP), &backgroundBitmap);
    background.width=backgroundBitmap.bmWidth;
    background.height=backgroundBitmap.bmHeight;
    //background Character

}
void HandleButtonClick(HWND hwnd, int mouseX, int mouseY, bool *main_menu, bool *intro)
{
    if (PtInRect(&startButton.rect, { mouseX, mouseY }))
    {
        KillTimer(hwnd, MAIN_MENU_TIMER); //kill the main menu timer
        SetTimer (hwnd, INTRO_TIMER, 32, NULL); //start the cutscene timer
        SetTimer (hwnd, INTRO_IMAGE_TIMER, 5000, NULL);
        playIntroMusic();
        *main_menu = false;
        *intro = true;
    }
    else if (PtInRect(&exitButton.rect, { mouseX, mouseY }))
    {
        exit(1);
    }
}

