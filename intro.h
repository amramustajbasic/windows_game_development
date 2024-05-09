#ifndef INTRO_H
#define INTRO_H
#include <tchar.h>
#include <windows.h>


void InitializeSkipButton(RECT clientRect);
void loadIntroBitmaps(RECT clientRect);
void DrawIntro(HDC hdc, RECT* prect);
void updateText();
void playIntroMusic();
void HandleIntroButtonClick(HWND hwnd, int mouseX, int mouseY, bool* intro, RECT clientRect);
void stopIntroMusic();

#endif // INTRO_H
