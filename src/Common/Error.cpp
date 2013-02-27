// ****************************************************************************
// Error.cpp
// ****************************************************************************

// ----------------------------------------------------------------------------
// INCLUDES
// ----------------------------------------------------------------------------

#include <windows.h>
#include <string>

#include "Error.h"

// ----------------------------------------------------------------------------
// CONSTANTS
// ----------------------------------------------------------------------------

const unsigned int MAX_USER_STRING_LENGTH = 256;

// ----------------------------------------------------------------------------
// FUNCTIONS
// ----------------------------------------------------------------------------

void DisplayErrorMessageToUser(const char* message, const char* param)
{
    if (param != NULL)
    {
        char fullMessage[MAX_USER_STRING_LENGTH];
        sprintf_s(fullMessage, MAX_USER_STRING_LENGTH, "%s: %s", message, param);
        MessageBox(NULL, fullMessage, "Error", MB_ICONERROR | MB_OK);
    }
    else
    {
        MessageBox(NULL, message, "Error", MB_ICONERROR | MB_OK);
    }
}

// ----------------------------------------------------------------------------

void DisplayWin32ErrorMessageToUser(const char* message)
{
    DWORD lastError = GetLastError();
    LPTSTR errorText = NULL;

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errorText, 0,  NULL);

    if (errorText != NULL)
    {
        char fullMessage[MAX_USER_STRING_LENGTH];
        sprintf_s(fullMessage, MAX_USER_STRING_LENGTH, "%s\n\n0x%X: %s", message, lastError, errorText);
        DisplayErrorMessageToUser(fullMessage);
        LocalFree(errorText);
    }
    else
    {
        DisplayErrorMessageToUser(message);
    }
}

// ----------------------------------------------------------------------------