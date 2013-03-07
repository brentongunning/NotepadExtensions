// ****************************************************************************
// UI.cpp
// ****************************************************************************

// ----------------------------------------------------------------------------
// INCLUDES
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "UI.h"
#include "Error.h"
#include "resource.h"

// ----------------------------------------------------------------------------
// STATIC MEMBER VARIABLES
// ----------------------------------------------------------------------------

UI* UI::_instance = NULL;

// ----------------------------------------------------------------------------
// METHODS
// ----------------------------------------------------------------------------

UI::UI() :
    _applicationInstance(NULL),
    _iconHandle(NULL),
    _windowHandle(NULL),
    _systemTrayPopupMenuHandle(NULL)
{
    ZeroMemory(&_systemTrayIconData, sizeof(NOTIFYICONDATA));
}

// ----------------------------------------------------------------------------

UI::~UI()
{
    // No-op
}

// ----------------------------------------------------------------------------

UI* UI::GetInstance()
{
    if (!UI::_instance)
        UI::_instance = new UI();
    return UI::_instance;
}

// ----------------------------------------------------------------------------

void UI::DeleteInstance()
{
    SAFE_DELETE(UI::_instance);
}

// ----------------------------------------------------------------------------

bool UI::Create(HINSTANCE applicationInstance)
{
    _applicationInstance = applicationInstance;

    return LoadMainIcon() &&
           RegisterWindowClass() &&
           CreateHiddenMainWindow() &&
           CreateSystemTrayMenu();
}

// ----------------------------------------------------------------------------

void UI::Run()
{
    MSG msg = {0};
    while(GetMessage(&msg, _windowHandle, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// ----------------------------------------------------------------------------

void UI::Destroy()
{
    Shell_NotifyIcon(NIM_DELETE, &_systemTrayIconData);
    DestroyWindow(_windowHandle);
    UnregisterClass(WINDOW_CLASS_NAME, _applicationInstance);
}

// ----------------------------------------------------------------------------

bool UI::LoadMainIcon()
{
    // Load the icon used for the application and system tray
    _iconHandle = LoadIcon(_applicationInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    if (!_iconHandle)
    {
        DisplayWin32ErrorMessageToUser("Failed to load main application icon");
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool UI::RegisterWindowClass()
{
    // Register the window class
    WNDCLASSEX wcx = {0};
    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = CS_OWNDC;
    wcx.lpfnWndProc = UI::StaticWinProc;
    wcx.hInstance = _applicationInstance;
    wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcx.hIcon = _iconHandle;
    wcx.hIconSm = _iconHandle;
    wcx.lpszClassName = WINDOW_CLASS_NAME;

    if (RegisterClassEx(&wcx) == NULL)
    {
        DisplayWin32ErrorMessageToUser("Failed to register window class");
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool UI::CreateHiddenMainWindow()
{
    // Create the hidden main window used to receive messages from the system tray menu
    _windowHandle = CreateWindowEx(WS_EX_TOOLWINDOW, WINDOW_CLASS_NAME, "Notepad Extensions", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, _applicationInstance, NULL);

    if (!_windowHandle)
    {
        DisplayWin32ErrorMessageToUser("Failed to create hidden main window");
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool UI::CreateSystemTrayMenu()
{
    // Create the system tray icon
    _systemTrayIconData.cbSize = sizeof(NOTIFYICONDATA);
    _systemTrayIconData.uFlags = NIF_MESSAGE | NIF_ICON;
    _systemTrayIconData.hWnd = _windowHandle;
    _systemTrayIconData.uID = TRAY_ICON_ID;
    _systemTrayIconData.uCallbackMessage = WM_SYSTEM_TRAY_MENU_EVENT_ID;
    _systemTrayIconData.hIcon = _iconHandle;
    if (Shell_NotifyIcon(NIM_ADD, &_systemTrayIconData) == FALSE)
    {
        DisplayErrorMessageToUser("Failed to create system tray icon");
        return false;
    }

    // Create the popup menu that will display when the tray icon is clicked
    if ((_systemTrayPopupMenuHandle = CreatePopupMenu()) == NULL ||
        AppendMenu(_systemTrayPopupMenuHandle, MF_STRING, TRAY_NEW_ID, "New") == FALSE ||
        AppendMenu(_systemTrayPopupMenuHandle, MF_STRING, TRAY_ABOUT_ID, "About") == FALSE ||
        AppendMenu(_systemTrayPopupMenuHandle, MF_STRING, TRAY_EXIT_ID, "Exit") == FALSE)
    {
        DisplayWin32ErrorMessageToUser("Failed to create system tray popup menu");
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------

LRESULT CALLBACK UI::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        {
            PostQuitMessage(0);
        } break;

    case WM_SYSTEM_TRAY_MENU_EVENT_ID:
        {
            if (lParam == WM_RBUTTONDOWN)
                OpenSystemTrayMenu();
        } break;

    case WM_COMMAND:
        {
            ProcessSystemTrayMenuEvent(LOWORD(wParam));
        } break;

    default:
        {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        } break;
    }

    return 0;
}

// ----------------------------------------------------------------------------

LRESULT CALLBACK UI::StaticWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return UI::GetInstance()->WinProc(hWnd, uMsg, wParam, lParam);
}

// ----------------------------------------------------------------------------

void UI::OpenSystemTrayMenu()
{
    POINT p;
    GetCursorPos(&p);
    SetForegroundWindow(_windowHandle);
    TrackPopupMenu(_systemTrayPopupMenuHandle, TPM_LEFTALIGN,
        p.x, p.y, 0, _windowHandle, 0);
    PostMessage(_windowHandle, WM_USER, 0, 0);
}

// ----------------------------------------------------------------------------

void UI::ProcessSystemTrayMenuEvent(DWORD id)
{
    switch(id)
    {
    case TRAY_NEW_ID:
        {
            ShellExecute(NULL, "open", "notepad.exe", "", NULL, SW_SHOWNORMAL);
        } break;

    case TRAY_ABOUT_ID:
        {
            DisplayAboutInformation();
        } break;

    case TRAY_EXIT_ID:
        {
            PostQuitMessage(0);
        } break;
    }
}

// ----------------------------------------------------------------------------

void UI::DisplayAboutInformation()
{
    MessageBox(NULL,
        "Extends the Windows Notepad program with a few additional features.\n"
        "\n"
        "Developed by Brenton Gunning\n",
        "About Notepad Extensions 1.2", MB_OK);
}

// ----------------------------------------------------------------------------