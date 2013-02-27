// ****************************************************************************
// NotepadHook.cpp
// ****************************************************************************

// ----------------------------------------------------------------------------
// INCLUDES
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "NotepadHook.h"
#include "Autosaver.h"
#include "TextEditor.h"
#include "Error.h"

// ----------------------------------------------------------------------------
// DEFINITIONS
// ----------------------------------------------------------------------------

// Used to detect key events
#define HK_IS_RELEASED    ((DWORD)lParam & (1 << 31))
#define HK_IS_HELD        ((DWORD)lParam & (1 << 30))
#define HK_IS_PRESSED   (!((DWORD)lParam & (1 << 31)))

// ----------------------------------------------------------------------------
// STATIC MEMBER VARIABLES
// ----------------------------------------------------------------------------

NotepadHook* NotepadHook::_instance = NULL;

// ----------------------------------------------------------------------------
// METHODS
// ----------------------------------------------------------------------------

NotepadHook::NotepadHook() :
    _windowHandle(NULL),
    _releaseHooksEvent(NULL),
    _keyboardHookHandle(NULL),
    _controlDown(false)
{
    // No-op
}

// ----------------------------------------------------------------------------

NotepadHook::~NotepadHook()
{
    // No-op
}

// ----------------------------------------------------------------------------

NotepadHook* NotepadHook::GetInstance()
{
    if (!_instance)
    {
        _instance = new NotepadHook();
    }

    return _instance;
}

// ----------------------------------------------------------------------------

bool NotepadHook::Install(HWND windowHandle)
{
    _windowHandle = windowHandle;

    return CheckIfNotepadMainWindow() &&
           InstallKeyboardHook() &&
           OpenReleaseHooksEvent() &&
           TextEditor::GetInstance()->Initialize(_windowHandle) &&
           Autosaver::GetInstance()->Start(_windowHandle);
}

// ----------------------------------------------------------------------------

void NotepadHook::Uninstall()
{
    Autosaver::GetInstance()->Stop();
    UnhookWindowsHookEx(_keyboardHookHandle);
}

// ----------------------------------------------------------------------------

bool NotepadHook::IsInstalled()
{
    return _keyboardHookHandle != NULL;
}

// ----------------------------------------------------------------------------

bool NotepadHook::InstallKeyboardHook()
{
    DWORD threadID = GetWindowThreadProcessId(_windowHandle, NULL);
    _keyboardHookHandle = SetWindowsHookEx(WH_KEYBOARD, StaticKeyboardProc,
        GetModuleHandle(NULL), threadID);
    if (_keyboardHookHandle == NULL)
    {
        DisplayWin32ErrorMessageToUser("Failed to hook keyboard events for notepad");
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool NotepadHook::OpenReleaseHooksEvent()
{
    _releaseHooksEvent = OpenEvent(SYNCHRONIZE | EVENT_ALL_ACCESS, FALSE, RELEASE_HOOKS_EVENT_NAME);
    if (!_releaseHooksEvent)
    {
        DisplayWin32ErrorMessageToUser("Failed to open release hooks event");
        return false;
    }
    else
    {
        // Create a thread to wait for the release hooks event
        if (!CreateThread(NULL, 0, ReleaseHooksThreadProc, NULL, 0, NULL))
        {
            DisplayWin32ErrorMessageToUser("Failed to create release hooks thread");
            return false;
        }
    }

    return true;
}

// ----------------------------------------------------------------------------

bool NotepadHook::CheckIfNotepadMainWindow()
{
    char title[MAX_USER_STRING_LENGTH];
    GetWindowText(_windowHandle, title, MAX_USER_STRING_LENGTH);

    if (string(title).find("- Notepad") != string::npos)
    {
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------

bool NotepadHook::OnInterceptKeyPress(DWORD virtualKey)
{
    switch (virtualKey)
    {
    case VK_CONTROL:
        {
            _controlDown = true;
        } break;

    case 0x57: // W
        {
            if (_controlDown)
                PostMessage(_windowHandle, WM_CLOSE, 0, 0);
        } break;

    case VK_RETURN:
        {
            return OnReleaseReturnKey();
        } break;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool NotepadHook::OnInterceptKeyRelease(DWORD virtualKey)
{
    switch (virtualKey)
    {
    case VK_RETURN:
        {
            return false;
        } break;

    case VK_CONTROL:
        {
            _controlDown = false;
        } break;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool NotepadHook::OnReleaseReturnKey()
{
    TextEditor::GetInstance()->InsertNewLineWithSameIndentation();
    return false;
}

// ----------------------------------------------------------------------------

DWORD WINAPI NotepadHook::ReleaseHooksThreadProc(LPVOID lpParameter)
{
    WaitForSingleObject(NotepadHook::GetInstance()->_releaseHooksEvent, INFINITE);
    NotepadHook::GetInstance()->Uninstall();
    return 0;
}

// ----------------------------------------------------------------------------

LRESULT NotepadHook::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0 || GetForegroundWindow() != _windowHandle)
        return CallNextHookEx(_keyboardHookHandle, nCode, wParam, lParam);

    if (nCode == HC_ACTION)
    {
        Autosaver::GetInstance()->OnUpdate();

        if (HK_IS_RELEASED)
        {
            if (!OnInterceptKeyRelease((DWORD)wParam))
                return 1;
            
        }
        else if (HK_IS_PRESSED)
        {
            if (!OnInterceptKeyPress((DWORD)wParam))
                return 1;
        }
    }

    return CallNextHookEx(_keyboardHookHandle, nCode, wParam, lParam);
}

// ----------------------------------------------------------------------------

LRESULT CALLBACK NotepadHook::StaticKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    return NotepadHook::GetInstance()->KeyboardProc(nCode, wParam, lParam);
}

// ----------------------------------------------------------------------------