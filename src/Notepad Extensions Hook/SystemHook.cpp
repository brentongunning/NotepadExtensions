// ****************************************************************************
// SystemHook.cpp
// ****************************************************************************

// ----------------------------------------------------------------------------
// INCLUDES
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "SystemHook.h"
#include "Error.h"
#include "NotepadHook.h"

// ----------------------------------------------------------------------------
// STATIC MEMBER VARIABLES
// ----------------------------------------------------------------------------

SystemHook* SystemHook::_instance = NULL;

// ----------------------------------------------------------------------------
// METHODS
// ----------------------------------------------------------------------------

SystemHook::SystemHook() :
    _dllHandle(false),
    _cbtHookHandle(NULL),
    _isNotepad(false)
{
    // No-op
}

// ----------------------------------------------------------------------------

SystemHook::~SystemHook()
{
    // No-op
}

// ----------------------------------------------------------------------------

SystemHook* SystemHook::GetInstance()
{
    if (!_instance)
    {
        _instance = new SystemHook();
    }

    return _instance;
}

// ----------------------------------------------------------------------------

void SystemHook::SetDllHandle(HINSTANCE dllHandle)
{
    _dllHandle = dllHandle;
}

// ----------------------------------------------------------------------------

bool SystemHook::Install()
{
    _cbtHookHandle = SetWindowsHookEx(WH_CBT, StaticCBTProc, _dllHandle, NULL);
    if (!_cbtHookHandle)
    {
        DisplayWin32ErrorMessageToUser("Failed to install CBT hook");
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------

void SystemHook::Uninstall()
{
    if (_cbtHookHandle)
    {
        UnhookWindowsHookEx(_cbtHookHandle);
        _cbtHookHandle = NULL;
    }
}

// ----------------------------------------------------------------------------

void SystemHook::OnDllAttach()
{
    // Get the path of the process exe and see if it is notepad

    char moduleName[MAX_USER_STRING_LENGTH];
    GetModuleFileName(NULL, moduleName, MAX_USER_STRING_LENGTH);

    for (unsigned int i = 0; i < strlen(moduleName); ++i)
        moduleName[i] = tolower(moduleName[i]);

    _isNotepad = string(moduleName).find("notepad.exe") != string::npos;
}

// ----------------------------------------------------------------------------

void SystemHook::OnDllDetach()
{
    if (_isNotepad && NotepadHook::GetInstance()->IsInstalled())
    {
        NotepadHook::GetInstance()->Uninstall();
    }
}

// ----------------------------------------------------------------------------

LRESULT SystemHook::CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (_isNotepad && nCode == HCBT_ACTIVATE)
    {
        if (!NotepadHook::GetInstance()->IsInstalled())
        {
            NotepadHook::GetInstance()->Install((HWND)wParam);
        }
    }

    return CallNextHookEx(_cbtHookHandle, nCode, wParam, lParam);
}

// ----------------------------------------------------------------------------

LRESULT CALLBACK SystemHook::StaticCBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    return SystemHook::GetInstance()->CBTProc(nCode, wParam, lParam);
}

// ----------------------------------------------------------------------------