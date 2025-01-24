#include "Leviathan.h"
#include "Graphics.h"

namespace Leviathan
{
#define APPNAMEW() (L"Leviathan")
#define APPNAMEA() ("Leviathan")
#if UNICODE
    #define APPNAME() APPNAMEW()
#else // UNICODE
    #define APPNAME() APPNAMEA()
#endif // UNICODE
    int AppWidth = 1280;
    int AppHeight = 720;
    bool bAppRunning = true;
    HWND AppWindow = nullptr;

    LRESULT WindowProc(
        HWND hWnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam 
    )
    {
        LRESULT lResult = 0;
        switch (uMsg)
        {
            case WM_KEYUP:
            {
                if (wParam == VK_ESCAPE)
                {
                    bAppRunning = false;
                }
            } break;
            case WM_CLOSE:
            {
                bAppRunning = false;
            }
            default:
            {
                lResult = DefWindowProc(hWnd, uMsg, wParam, lParam);
            } break;
        }
        return lResult;
    }

    HWND InitWindow(HINSTANCE hInst, int Width, int Height)
    {
        WNDCLASSEX WndClassEx = {};
        WndClassEx.cbSize = sizeof(WNDCLASSEXA);
        WndClassEx.style = CS_HREDRAW | CS_VREDRAW;
        WndClassEx.lpfnWndProc = WindowProc;
        WndClassEx.cbClsExtra = 0;
        WndClassEx.cbWndExtra = 0;
        WndClassEx.hIcon = nullptr;
        WndClassEx.hCursor = nullptr;
        WndClassEx.hbrBackground = nullptr;
        WndClassEx.lpszMenuName = nullptr;
        WndClassEx.lpszClassName = APPNAME();
        WndClassEx.hIconSm = nullptr;
        RegisterClassEx(&WndClassEx);

        DWORD WndExStyle = WS_EX_OVERLAPPEDWINDOW;
        DWORD WndStyle = WS_CAPTION | WS_OVERLAPPEDWINDOW;

        RECT WndRect = { 0, 0, (LONG)Width, (LONG)Height };
        AdjustWindowRectEx(&WndRect, WndStyle, FALSE, WndExStyle);
        // CKA_NOTE: AdjustWindowRectExForDpi
        LONG AdjustedWidth = WndRect.right - WndRect.left;
        LONG AdjustedHeight = WndRect.bottom - WndRect.top;

        return CreateWindowEx(
            WndExStyle,
            APPNAME(),
            APPNAME(),
            WndStyle,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            AdjustedWidth,
            AdjustedHeight,
            nullptr,
            nullptr,
            hInst,
            nullptr /* lpParam */
        );
    }

    int WindowMsgLoop(HWND InWindow)
    {
        int MsgCount = 0;
        MSG Msg = {};
        while (PeekMessage(&Msg, InWindow, 0, 0, PM_REMOVE) > 0)
        {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
            MsgCount++;
        }
        return MsgCount;
    }

    int WinMain
    (
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR     lpCmdLine,
        int       nShowCmd
    )
    {
        OutputDebugStringA("LEVIATHAN -- INIT\n");

        HWND hWnd = InitWindow(hInstance, AppWidth, AppHeight);
        if (hWnd)
        {
            AppWindow = hWnd;
            ShowWindow(AppWindow, nShowCmd);

            InitGraphics();

            bAppRunning = true;
            while (bAppRunning)
            {
                WindowMsgLoop(AppWindow);
                UpdateAndDraw();
            }

            TermGraphics();
        }

        return 0;
    }
}

int WinMain
(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
)
{
    return Leviathan::WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

