// ****************************************************************************
// Hook.cpp
// ****************************************************************************

// ----------------------------------------------------------------------------
// INCLUDES
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "Hook.h"
#include "../Common/Error.h"

// ----------------------------------------------------------------------------
// METHODS
// ----------------------------------------------------------------------------

Hook::Hook() :
    InstallHook(0),
    UninstallHook(0),
    _releaseHooksEvent(NULL)
{
    // No-op
}

// ----------------------------------------------------------------------------

Hook::~Hook()
{
    // No-op
}

// ----------------------------------------------------------------------------

bool Hook::Install()
{
    HINSTANCE hookInstance = LoadLibrary(HOOK_DLL); 
    if (hookInstance == NULL) 
    {
        DisplayErrorMessageToUser("Failed to load hook DLL", HOOK_DLL);
        return false;
    } 

    InstallHook = (InstallHookFunction)GetProcAddress(hookInstance, "InstallHook");
    UninstallHook = (UninstallHookFunction)GetProcAddress(hookInstance, "UninstallHook");

    if (!InstallHook || !UninstallHook)
    {
        DisplayErrorMessageToUser("Failed to find hook exported functions");
        return false;
    }

    if (!InstallHook())
    {
        DisplayErrorMessageToUser("Failed to install system-wide hook");
        return false;
    }

    _releaseHooksEvent = CreateEvent(NULL, TRUE, FALSE, RELEASE_HOOKS_EVENT_NAME);
    if (!_releaseHooksEvent)
    {
        DisplayWin32ErrorMessageToUser("Failed to create release hooks event");
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------

void Hook::Uninstall()
{
    SetEvent(_releaseHooksEvent);
    CloseHandle(_releaseHooksEvent);

    if (UninstallHook)
    {
        UninstallHook();
    }
}

// ----------------------------------------------------------------------------