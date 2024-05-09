#ifndef MAINMENU_H
#define MAINMENU_H
#include <windows.h>
#include "object.h"

void DrawMainMenu(HDC hdc, RECT* prect);
void InitializeButtons(RECT clientRect);
void loadMainMenuBitmaps(RECT);
void HandleButtonClick(HWND hwnd, int mouseX, int mouseY, bool *main_menu, bool* intro);



#endif // MAINMENU_H
