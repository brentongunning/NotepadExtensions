// ****************************************************************************
// UI.h
// ****************************************************************************

#pragma once

// ----------------------------------------------------------------------------
// CONSTANTS
// ----------------------------------------------------------------------------

#define WINDOW_CLASS_NAME           "NotepadExtensionsWindowClass"
#define WM_SYSTEM_TRAY_MENU_EVENT_ID            WM_USER + 2
#define TRAY_ICON_ID                1000
#define TRAY_NEW_ID                 1001
#define TRAY_ABOUT_ID               1002
#define TRAY_EXIT_ID                1003

// ----------------------------------------------------------------------------
// CLASSES
// ----------------------------------------------------------------------------

// The one and only user interface, the system tray
class UI
{
protected:
    UI();
    ~UI();

public:
    static UI* GetInstance();
    static void DeleteInstance();

public:
    bool Create(HINSTANCE applicationInstance);
    void Run();
    void Destroy();

    // Initialization methods
    bool LoadMainIcon();
    bool RegisterWindowClass();
    bool CreateHiddenMainWindow();
    bool CreateSystemTrayMenu();

    // StaticWinProc redirects to our local WinProc method.
    LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK StaticWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Other helper methods
    void OpenSystemTrayMenu();
    void ProcessSystemTrayMenuEvent(DWORD id);
    void DisplayAboutInformation();

private:
    static UI* _instance;

    HINSTANCE _applicationInstance;
    HICON _iconHandle;
    HWND _windowHandle;
    NOTIFYICONDATA _systemTrayIconData;
    HMENU _systemTrayPopupMenuHandle;
};

// ----------------------------------------------------------------------------