// ****************************************************************************
// DllMain.cpp
// ****************************************************************************

#include "stdafx.h"
#include "SystemHook.h"

// ----------------------------------------------------------------------------
// FUNCTIONS
// ----------------------------------------------------------------------------

extern "C" __declspec(dllexport) bool InstallHook()
{
    return SystemHook::GetInstance()->Install();
}

// ----------------------------------------------------------------------------

extern "C" __declspec(dllexport) void UninstallHook()
{
    SystemHook::GetInstance()->Uninstall();
}

// ----------------------------------------------------------------------------

BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
{
    SystemHook::GetInstance()->SetDllHandle((HINSTANCE)hDllHandle);

    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        SystemHook::GetInstance()->OnDllAttach();
        break;
    case DLL_PROCESS_DETACH:
        SystemHook::GetInstance()->OnDllDetach();
        break;
    }

    return TRUE;
}

// ----------------------------------------------------------------------------