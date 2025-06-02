#include "Leviathan.h" // NOTE: Engine.h is included in Leviathan.h
#include "Graphics.h"
#include "Clock.h"
#include "Game/GameManager.h"

namespace Leviathan
{

unsigned int FrameNumber = 0u;

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
        case WM_CLOSE:
        {
            bAppRunning = false;
        } break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            KeyboardState::Win32_KeyMsg(uMsg, wParam, lParam);
        } break;
        case WM_MOUSEMOVE:
        case WM_MOUSELEAVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
        case WM_MOUSEWHEEL:
        {
            MouseState::Win32_MouseMsg(uMsg, wParam, lParam);
        } break;
        case WM_INPUT:
        {
            RawInputHandler::Win32_ProcessInput(wParam, lParam);
        } break;
        case WM_INPUT_DEVICE_CHANGE:
        {
            RawInputHandler::Win32_DeviceChange(wParam, lParam);
        } break;
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
    WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
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

void Engine::Init(HINSTANCE hInstance)
{
#if ENABLE_MEMORY_CHECK()
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif // ENABLE_MEMORY_CHECK()

    HWND hWnd = InitWindow(hInstance, AppWidth, AppHeight);
    if (hWnd)
    {
        AppWindow = hWnd;
        ASSERT(AppWindow != nullptr);
        int WndShowFlag = SW_SHOWNORMAL;
        ShowWindow(AppWindow, WndShowFlag);

        Graphics::Init();
        RawInputHandler::Init();
        Clock::Init();
        GameManager::Init();
    }
}

void Engine::MainLoop()
{
    bAppRunning = true;
    FrameNumber++;
    while (bAppRunning)
    {
        WindowMsgLoop(AppWindow);
        GameManager::Update();
        Graphics::Draw();

        if (KeyboardState::GetKey(LV_KEY_ESC))
        {
            bAppRunning = false;
        }

        KeyboardState::Tick();
        MouseState::Tick();
        GamepadState::Tick();
        Clock::Tick();
    }
}

void Engine::Term()
{
    GameManager::Term();
    Graphics::Term();
}

int WinMain
(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
)
{
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nShowCmd;

    Engine::Init(hInstance);
    Engine::MainLoop();
    Engine::Term();

    return 0;
}

} // namespace Leviathan

