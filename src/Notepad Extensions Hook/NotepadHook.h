// ****************************************************************************
// NotepadHook.h
// ****************************************************************************

#pragma once

// ----------------------------------------------------------------------------
// CONSTANTS
// ----------------------------------------------------------------------------

#define RELEASE_HOOKS_EVENT_NAME "NotepadExtensionsReleaseHooksEvent"
#define TIME_DIFFERENCE_TO_RECORD_IN_MINUTES 5

// ----------------------------------------------------------------------------
// CLASSES
// ----------------------------------------------------------------------------

// The hook into notepad instances that monitors for key presses.
class NotepadHook
{
protected:
    NotepadHook();
    ~NotepadHook();

public:
    static NotepadHook* GetInstance();

    bool Install(HWND windowHandle);
    void Uninstall();
    bool IsInstalled();

private:
    // Initialization methods
    bool InstallKeyboardHook();
    bool OpenReleaseHooksEvent();

    bool CheckIfNotepadMainWindow();

    // Returns true for letting the key through, false for swallowing the event.
    bool OnInterceptKeyPress(DWORD virtualKey);
    bool OnInterceptKeyRelease(DWORD virtualKey);
    bool OnReleaseReturnKey();

    static DWORD WINAPI ReleaseHooksThreadProc(LPVOID lpParameter);
    LRESULT KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK StaticKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
    static NotepadHook* _instance;

    HWND _windowHandle;
    HANDLE _releaseHooksEvent;
    HHOOK _keyboardHookHandle;
    bool _controlDown;
};

// ----------------------------------------------------------------------------