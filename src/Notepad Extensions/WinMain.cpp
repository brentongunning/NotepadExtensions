// ****************************************************************************
// WinMain.cpp
// ****************************************************************************

// ----------------------------------------------------------------------------
// INCLUDES
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "Application.h"

// ----------------------------------------------------------------------------
// FUNCTIONS
// ----------------------------------------------------------------------------

// Entry point for the program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    CoInitialize(0);
    Application* application = new Application(hInstance);

    application->Run();

    SAFE_DELETE(application);
    CoUninitialize();
    return 0;
}

// ----------------------------------------------------------------------------