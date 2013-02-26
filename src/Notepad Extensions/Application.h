// ****************************************************************************
// Application.h
// ****************************************************************************

#pragma once

// ----------------------------------------------------------------------------
// INCLUDES
// ----------------------------------------------------------------------------

#include "UI.h"
#include "Hook.h"

// ----------------------------------------------------------------------------
// CONSTANTS
// ----------------------------------------------------------------------------

#define SINGLE_PROCESS_MUTEX_NAME   "xLyMutex"

// ----------------------------------------------------------------------------
// CLASSES
// ----------------------------------------------------------------------------

// Starts, runs, and shuts down the core program logic
class Application
{
public:
    Application(HINSTANCE hInstance);
    ~Application();

    void Run();

private:
    bool Init();
    void Shutdown();

    // Initialization methods
    bool EnsureOnlyOneProcess();

private:
    HINSTANCE _applicationInstance;

    HANDLE _singleProcessMutex;         // Used to guarantee only one program instance
    Hook _hook;
};

// ----------------------------------------------------------------------------