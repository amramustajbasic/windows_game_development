#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <mmsystem.h>
#include <ctime>
#include <random>
#include "mainMenu.h"
#include "intro.h"
#include "game_logic.h"
#include "resetGame.h"



/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = (HICON)LoadImage(hThisInstance, "game_icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    wincl.hIconSm = (HICON)LoadImage(hThisInstance, "game_icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Captain Holetooth"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               716,                 /* The programs width */
               540,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    static HDC hdc;
    static bool main_menu=false;
    static bool intro = false;
    static RECT clientRect;
    switch (message)                  /* handle the messages */
    {
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    case WM_CREATE:
        std::srand(static_cast<unsigned> (time(nullptr)));
        GetClientRect(hwnd, &clientRect);
        loadMainMenuBitmaps(clientRect);
        loadIntroBitmaps(clientRect);
        loadGameBitmaps(clientRect);
        SetTimer (hwnd, MAIN_MENU_TIMER, 100, NULL);
        main_menu=true;

        break;
    case WM_TIMER:
        switch(wParam)
        {
        case MAIN_MENU_TIMER:
            hdc = GetDC(hwnd);
            DrawMainMenu(hdc, &clientRect);
            InitializeButtons(clientRect);
            ReleaseDC(hwnd, hdc);
            break;
        case INTRO_TIMER:
            hdc = GetDC(hwnd);
            InitializeSkipButton(clientRect);
            DrawIntro(hdc, &clientRect);
            updateText();
            ReleaseDC(hwnd, hdc);
            break;
        case INTRO_IMAGE_TIMER:

            break;
        case GAME_TIMER:
            hdc = GetDC(hwnd);
            showBackground(hdc, &clientRect);
            applyGravity();
            updateEnemiesMovement();

            if (GetAsyncKeyState(VK_UP) & 0x8000)
            {
                updatePlayer(JUMP,  clientRect);
            }
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            {
                updatePlayer(RIGHT,  clientRect);
            }
            if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            {
                updatePlayer(LEFT, clientRect);
            }
             if (GetAsyncKeyState(VK_SPACE) & 0x8000)
            {
                updatePlayer(SHOOT, clientRect);
            }
            ReleaseDC(hwnd, hdc);
            break;
        case CLOCK_TIMER:
            updateClock();
            break;
        }
        break;
        case WM_KEYDOWN:
    switch (wParam)
    {
    case 'R':
    case 'r':
        resetGame(hwnd, clientRect);
        break;
    case 'Q':
    case 'q':
        KillTimer(hwnd,GAME_TIMER);
        SetTimer(hwnd,MAIN_MENU_TIMER,100,NULL);
        main_menu = true;
        break;

    }
    break;
    case WM_LBUTTONDOWN:
        if(main_menu)
        {
            HandleButtonClick(hwnd, LOWORD(lParam), HIWORD(lParam), &main_menu, &intro);
        }
        if(intro)
        {
            HandleIntroButtonClick(hwnd, LOWORD(lParam), HIWORD(lParam), &intro, clientRect);
        }
        break;
    default:
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;

}


