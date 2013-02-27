// ****************************************************************************
// Application.cpp
// ****************************************************************************

// ----------------------------------------------------------------------------
// INCLUDES
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "Application.h"
#include "Error.h"

// ----------------------------------------------------------------------------
// METHODS
// ----------------------------------------------------------------------------

Application::Application(HINSTANCE hInstance) :
    _applicationInstance(hInstance),
    _singleProcessMutex(NULL)
{
}

// ----------------------------------------------------------------------------

Application::~Application()
{
    // No-op
}

// ----------------------------------------------------------------------------

void Application::Run()
{
    if (Init())
    {
        UI::GetInstance()->Run();
    }

    Shutdown();
}

// ----------------------------------------------------------------------------

bool Application::Init()
{
    return EnsureOnlyOneProcess() &&
            _hook.Install() &&
           UI::GetInstance()->Create(_applicationInstance);
}

// ----------------------------------------------------------------------------

void Application::Shutdown()
{
    UI::GetInstance()->Destroy();
    UI::DeleteInstance();
    _hook.Uninstall();
    CloseHandle(_singleProcessMutex);
}

// ----------------------------------------------------------------------------

bool Application::EnsureOnlyOneProcess()
{
    _singleProcessMutex = CreateMutex(0, TRUE, SINGLE_PROCESS_MUTEX_NAME);
    if (!_singleProcessMutex)
    {
        DisplayWin32ErrorMessageToUser("Failed to create the single process mutex");
        return false;
    }

    // The mutex may be created anyway if it already exists
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        DisplayErrorMessageToUser("This program is already running");
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------