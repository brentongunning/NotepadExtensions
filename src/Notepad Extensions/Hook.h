// ****************************************************************************
// Hook.h
// ****************************************************************************

#pragma once

// ----------------------------------------------------------------------------
// DEFINITIONS
// ----------------------------------------------------------------------------

#define HOOK_DLL "Notepad Extensions Hook.dll"
#define RELEASE_HOOKS_EVENT_NAME "NotepadExtensionsReleaseHooksEvent"

// ----------------------------------------------------------------------------
// CLASSES
// ----------------------------------------------------------------------------

// Loads and unloads the Win32 hook into notepad
class Hook
{
public:
    Hook();
    ~Hook();

    bool Install();
    void Uninstall();

private:
    typedef bool (*InstallHookFunction)();
    typedef void (*UninstallHookFunction)();

    InstallHookFunction InstallHook;
    UninstallHookFunction UninstallHook;

    // Tells hooked notepad programs to unhook when shutting down
    HANDLE _releaseHooksEvent;
};

// ----------------------------------------------------------------------------